CC ?= cc
CXX ?= c++
AR ?= ar
PREFIX ?= /usr/local

CPPFLAGS += -Iinclude -D_POSIX_C_SOURCE=200809L
CFLAGS ?= -O2
CFLAGS += -std=c11 -Wall -Wextra

PKG_CFLAGS := $(shell pkg-config --cflags glfw3 2>/dev/null)
PKG_LIBS := $(shell pkg-config --libs glfw3 2>/dev/null)
UNAME_S := $(shell uname -s)

CPPFLAGS += $(PKG_CFLAGS)

ifeq ($(UNAME_S),Darwin)
PLATFORM_LIBS := -framework Cocoa -framework IOKit -framework CoreVideo
else
PLATFORM_LIBS := -lm -ldl -lpthread
endif

LIBS := $(PKG_LIBS) $(PLATFORM_LIBS)

BUILD := build
LIB := $(BUILD)/liblwcgl.a
OBJ := $(BUILD)/lwcgl.o $(BUILD)/memory.o $(BUILD)/glfw.o $(BUILD)/opengl.o

.PHONY: all clean install uninstall check check-deps deps

all: check-deps $(LIB)

check-deps:
	@command -v pkg-config >/dev/null 2>&1 || { \
		echo "error: pkg-config is required"; \
		echo "run: make deps"; \
		exit 1; \
	}
	@pkg-config --exists glfw3 || { \
		echo "error: GLFW 3 development files are missing"; \
		echo "run: make deps"; \
		exit 1; \
	}

deps:
ifeq ($(UNAME_S),Darwin)
	@command -v brew >/dev/null 2>&1 || { echo "error: Homebrew is required"; exit 1; }
	HOMEBREW_NO_AUTO_UPDATE=1 brew install pkg-config glfw
else
	@set -e; \
	if command -v apt-get >/dev/null 2>&1; then \
		sudo apt-get update; \
		sudo apt-get install -y build-essential pkg-config libglfw3-dev; \
	elif command -v pacman >/dev/null 2>&1; then \
		sudo pacman -S --needed base-devel pkgconf glfw-x11; \
	elif command -v dnf >/dev/null 2>&1; then \
		sudo dnf install -y gcc gcc-c++ make pkgconf-pkg-config glfw-devel; \
	elif command -v zypper >/dev/null 2>&1; then \
		sudo zypper install -y gcc gcc-c++ make pkg-config libglfw3-devel; \
	else \
		echo "error: unsupported package manager"; \
		echo "install GLFW 3, pkg-config and a C/C++ toolchain manually"; \
		exit 1; \
	fi
endif

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/lwcgl.o: src/lwcgl.c src/lwcgl_internal.h include/lwcgl/lwcgl.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c src/lwcgl.c -o $@

$(BUILD)/memory.o: src/memory.c src/lwcgl_internal.h include/lwcgl/lwcgl.h include/lwcgl/memory.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c src/memory.c -o $@

$(BUILD)/glfw.o: src/glfw.c include/lwcgl/lwcgl.h include/lwcgl/glfw.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c src/glfw.c -o $@

$(BUILD)/opengl.o: src/opengl.c src/lwcgl_internal.h include/lwcgl/lwcgl.h include/lwcgl/opengl.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c src/opengl.c -o $@

$(LIB): $(OBJ)
	$(AR) rcs $@ $^

check: all
	$(CC) $(CPPFLAGS) $(CFLAGS) tests/version_contract.c $(LIB) $(LIBS) -o $(BUILD)/version-contract
	$(BUILD)/version-contract
	$(CC) $(CPPFLAGS) $(CFLAGS) tests/lwjgl3_contract.c $(LIB) $(LIBS) -o $(BUILD)/lwjgl3-contract-c
	$(CXX) $(CPPFLAGS) -std=c++17 -Wall -Wextra tests/lwjgl3_contract.cpp $(LIB) $(LIBS) -o $(BUILD)/lwjgl3-contract-cpp

install: $(LIB)
	install -d $(DESTDIR)$(PREFIX)/include/lwcgl
	install -m 0644 include/lwcgl/lwcgl.h $(DESTDIR)$(PREFIX)/include/lwcgl/lwcgl.h
	install -m 0644 include/lwcgl/memory.h $(DESTDIR)$(PREFIX)/include/lwcgl/memory.h
	install -m 0644 include/lwcgl/glfw.h $(DESTDIR)$(PREFIX)/include/lwcgl/glfw.h
	install -m 0644 include/lwcgl/opengl.h $(DESTDIR)$(PREFIX)/include/lwcgl/opengl.h
	install -d $(DESTDIR)$(PREFIX)/lib
	install -m 0644 $(LIB) $(DESTDIR)$(PREFIX)/lib/liblwcgl.a

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/include/lwcgl/lwcgl.h
	rm -f $(DESTDIR)$(PREFIX)/include/lwcgl/memory.h
	rm -f $(DESTDIR)$(PREFIX)/include/lwcgl/glfw.h
	rm -f $(DESTDIR)$(PREFIX)/include/lwcgl/opengl.h
	rmdir $(DESTDIR)$(PREFIX)/include/lwcgl 2>/dev/null || true
	rm -f $(DESTDIR)$(PREFIX)/lib/liblwcgl.a

clean:
	rm -rf $(BUILD)
