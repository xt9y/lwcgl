CC ?= cc
CXX ?= c++
AR ?= ar
PREFIX ?= /usr/local
VERSION := 2.9.3
ABI := 2
BUILD := build
LIBNAME := liblwcgl-$(VERSION).a
LIB := $(BUILD)/$(LIBNAME)
PUBLIC_HEADERS := $(wildcard include/lwcgl/*.h)
SRC := $(wildcard src/*.c)
OBJ := $(patsubst src/%.c,$(BUILD)/%.o,$(SRC))
CONFIG := $(BUILD)/.build-config
PKGCONFIG := $(BUILD)/lwcgl-$(VERSION).pc

CPPFLAGS += -Iinclude -D_POSIX_C_SOURCE=200809L
CFLAGS ?= -O2
CFLAGS += -std=c11 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -O2
CXXFLAGS += -std=c++17 -Wall -Wextra -Wpedantic
LDFLAGS ?=

PKG_CFLAGS := $(shell pkg-config --cflags glfw3 2>/dev/null)
PKG_LIBS := $(shell pkg-config --libs glfw3 2>/dev/null)
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
CPPFLAGS += -DGL_SILENCE_DEPRECATION
PLATFORM_LIBS := -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
PRIVATE_LIBS_PC := -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
else ifeq ($(UNAME_S),Linux)
PLATFORM_LIBS := -lGL -lGLU -lm -ldl -lpthread
PRIVATE_LIBS_PC := -lGL -lGLU -lm -ldl -lpthread
else
$(error unsupported host OS: $(UNAME_S); supported hosts are Linux and macOS)
endif
CPPFLAGS += $(PKG_CFLAGS)
LIBS := $(PKG_LIBS) $(PLATFORM_LIBS)
TEST_DIR := $(BUILD)/tests
TEST_BINS := $(TEST_DIR)/api-contract $(TEST_DIR)/buffer-contract $(TEST_DIR)/native-header-order $(TEST_DIR)/compat-contract $(TEST_DIR)/runtime-smoke

.PHONY: all clean check test install uninstall check-deps deps stage-check FORCE
all: check-deps $(LIB)
test: check

check-deps:
	@command -v pkg-config >/dev/null 2>&1 || { echo "error: pkg-config is required"; exit 1; }
	@pkg-config --atleast-version=3.3 glfw3 || { echo "error: GLFW >= 3.3 development files are required"; exit 1; }
ifeq ($(UNAME_S),Linux)
	@printf '#include <GL/gl.h>\n#include <GL/glu.h>\n' | $(CC) -x c -E - >/dev/null 2>&1 || { echo "error: OpenGL and GLU development headers are required"; exit 1; }
endif

deps:
ifeq ($(UNAME_S),Darwin)
	HOMEBREW_NO_AUTO_UPDATE=1 brew install pkg-config glfw
else
	@set -e; if command -v apt-get >/dev/null; then sudo apt-get update && sudo apt-get install -y build-essential pkg-config libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev xvfb; elif command -v pacman >/dev/null; then sudo pacman -S --needed base-devel pkgconf glfw-x11 mesa glu xorg-server-xvfb; elif command -v dnf >/dev/null; then sudo dnf install -y gcc gcc-c++ make pkgconf-pkg-config glfw-devel mesa-libGL-devel mesa-libGLU-devel xorg-x11-server-Xvfb; else echo "error: install GLFW >= 3.3, OpenGL, GLU and pkg-config manually"; exit 1; fi
endif

$(BUILD) $(TEST_DIR):
	mkdir -p $@

FORCE:
$(CONFIG): FORCE | $(BUILD)
	@{ printf '%s\n' 'CC=$(CC)' 'CXX=$(CXX)' 'CPPFLAGS=$(CPPFLAGS)' 'CFLAGS=$(CFLAGS)' 'CXXFLAGS=$(CXXFLAGS)' 'LDFLAGS=$(LDFLAGS)' 'SRC=$(SRC)' 'HEADERS=$(PUBLIC_HEADERS)'; } > $@.tmp
	@cmp -s $@.tmp $@ 2>/dev/null || mv $@.tmp $@
	@rm -f $@.tmp

$(BUILD)/keyboard.o: src/key_table.inc

$(BUILD)/%.o: src/%.c $(PUBLIC_HEADERS) src/internal.h $(CONFIG) | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -Werror -c $< -o $@

$(LIB): $(OBJ)
	rm -f $@
	$(AR) rcs $@ $^

$(PKGCONFIG): lwcgl-2.9.3.pc.in | $(BUILD)
	sed -e 's|@PREFIX@|$(PREFIX)|g' -e 's|@PRIVATE_LIBS@|$(PRIVATE_LIBS_PC)|g' $< > $@

$(TEST_DIR)/api-contract: tests/api_contract.cpp $(LIB) | $(TEST_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -Werror $< $(LIB) $(LDFLAGS) $(LIBS) -o $@
$(TEST_DIR)/buffer-contract: tests/buffer_contract.cpp $(LIB) | $(TEST_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -Werror $< $(LIB) $(LDFLAGS) $(LIBS) -o $@
$(TEST_DIR)/native-header-order: tests/native_header_order.c $(LIB) | $(TEST_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -Werror $< $(LIB) $(LDFLAGS) $(LIBS) -o $@
$(TEST_DIR)/compat-contract: tests/compat_contract.cpp $(LIB) | $(TEST_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -Werror $< $(LIB) $(LDFLAGS) $(LIBS) -o $@
$(TEST_DIR)/runtime-smoke: tests/runtime_smoke.c $(LIB) | $(TEST_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -Werror $< $(LIB) $(LDFLAGS) $(LIBS) -o $@

check: check-deps $(TEST_BINS) stage-check
	$(TEST_DIR)/api-contract
	$(TEST_DIR)/buffer-contract
	$(TEST_DIR)/native-header-order
	$(TEST_DIR)/compat-contract
	$(TEST_DIR)/runtime-smoke

stage-check: $(LIB) $(PKGCONFIG)
	rm -rf $(BUILD)/stage
	$(MAKE) install DESTDIR=$(abspath $(BUILD)/stage) PREFIX=/usr
	PKG_CONFIG_PATH=$(abspath $(BUILD)/stage)/usr/lib/pkgconfig PKG_CONFIG_SYSROOT_DIR=$(abspath $(BUILD)/stage) $(CC) $(CFLAGS) tests/stage_consumer.c $$(PKG_CONFIG_PATH=$(abspath $(BUILD)/stage)/usr/lib/pkgconfig PKG_CONFIG_SYSROOT_DIR=$(abspath $(BUILD)/stage) pkg-config --cflags --libs lwcgl-$(VERSION)) -o $(TEST_DIR)/stage-consumer-c
	PKG_CONFIG_PATH=$(abspath $(BUILD)/stage)/usr/lib/pkgconfig PKG_CONFIG_SYSROOT_DIR=$(abspath $(BUILD)/stage) $(CXX) $(CXXFLAGS) tests/stage_consumer.cpp $$(PKG_CONFIG_PATH=$(abspath $(BUILD)/stage)/usr/lib/pkgconfig PKG_CONFIG_SYSROOT_DIR=$(abspath $(BUILD)/stage) pkg-config --cflags --libs lwcgl-$(VERSION)) -o $(TEST_DIR)/stage-consumer-cpp
	$(TEST_DIR)/stage-consumer-c
	$(TEST_DIR)/stage-consumer-cpp

install: check-deps $(LIB)
	install -d $(DESTDIR)$(PREFIX)/include/lwcgl-$(VERSION)/lwcgl
	install -m 0644 $(PUBLIC_HEADERS) $(DESTDIR)$(PREFIX)/include/lwcgl-$(VERSION)/lwcgl/
	install -d $(DESTDIR)$(PREFIX)/lib/pkgconfig
	install -m 0644 $(LIB) $(DESTDIR)$(PREFIX)/lib/$(LIBNAME)
	@sed -e 's|@PREFIX@|$(PREFIX)|g' -e 's|@PRIVATE_LIBS@|$(PRIVATE_LIBS_PC)|g' lwcgl-2.9.3.pc.in > $(DESTDIR)$(PREFIX)/lib/pkgconfig/lwcgl-$(VERSION).pc

uninstall:
	rm -rf $(DESTDIR)$(PREFIX)/include/lwcgl-$(VERSION)
	rm -f $(DESTDIR)$(PREFIX)/lib/$(LIBNAME) $(DESTDIR)$(PREFIX)/lib/pkgconfig/lwcgl-$(VERSION).pc

clean:
	rm -rf $(BUILD)
