CC ?= cc
CXX ?= c++
AR ?= ar
PREFIX ?= /usr/local

BUILD := build
LIB := $(BUILD)/liblwcgl.a
PUBLIC_HEADERS := $(wildcard include/lwcgl/*.h)
SRC := $(wildcard src/*.c)
OBJ := $(patsubst src/%.c,$(BUILD)/%.o,$(SRC))

CPPFLAGS += -Iinclude -D_POSIX_C_SOURCE=200809L
CFLAGS ?= -O2
CFLAGS += -std=c11 -Wall -Wextra
CXXFLAGS ?= -O2
CXXFLAGS += -std=c++17 -Wall -Wextra

PKG_CFLAGS := $(shell pkg-config --cflags glfw3 2>/dev/null)
PKG_LIBS := $(shell pkg-config --libs glfw3 2>/dev/null)
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
CPPFLAGS += -DGL_SILENCE_DEPRECATION
PLATFORM_LIBS := -framework Cocoa -framework IOKit -framework CoreVideo
else
PLATFORM_LIBS := -lm -ldl -lpthread
endif

CPPFLAGS += $(PKG_CFLAGS)
LIBS := $(PKG_LIBS) $(PLATFORM_LIBS)

TEST_DIR := $(BUILD)/tests
TEST_BINS := \
	$(TEST_DIR)/version-contract \
	$(TEST_DIR)/lwjgl3-contract-c \
	$(TEST_DIR)/lwjgl3-contract-cpp

.PHONY: all clean test install uninstall check-deps deps

all: check-deps $(LIB)

check-deps:
ifeq ($(UNAME_S),Darwin)
	@command -v pkg-config >/dev/null 2>&1 || { \
		echo "error: pkg-config is required"; \
		echo "install with: brew install pkg-config glfw"; \
		exit 1; \
	}
	@pkg-config --exists glfw3 || { \
		echo "error: GLFW 3 development files are missing"; \
		echo "install with: brew install glfw"; \
		exit 1; \
	}
else
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
endif

deps:
ifeq ($(UNAME_S),Darwin)
	@command -v brew >/dev/null 2>&1 || { echo "error: Homebrew is required to install dependencies automatically"; exit 1; }
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
		sudo zypper install -y gcc gcc-c++ make pkg-config glfw-devel; \
	else \
		echo "error: unsupported package manager"; \
		echo "install GLFW 3, pkg-config and a C/C++ toolchain manually"; \
		exit 1; \
	fi
endif

$(BUILD):
	mkdir -p $(BUILD)

$(TEST_DIR):
	mkdir -p $(TEST_DIR)

$(BUILD)/%.o: src/%.c $(PUBLIC_HEADERS) src/lwcgl_internal.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(LIB): $(OBJ)
	$(AR) rcs $@ $^

$(TEST_DIR)/version-contract: tests/version_contract.c $(LIB) | $(TEST_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(LIB) $(LIBS) -o $@

$(TEST_DIR)/lwjgl3-contract-c: tests/lwjgl3_contract.c $(LIB) | $(TEST_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(LIB) $(LIBS) -o $@

$(TEST_DIR)/lwjgl3-contract-cpp: tests/lwjgl3_contract.cpp $(LIB) | $(TEST_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< $(LIB) $(LIBS) -o $@

test: check-deps $(TEST_BINS)
	@set -e; \
	for test_bin in $(TEST_BINS); do \
		echo "RUN $$test_bin"; \
		"$$test_bin"; \
	done

install: check-deps $(LIB)
	install -d $(DESTDIR)$(PREFIX)/include/lwcgl
	install -m 0644 $(PUBLIC_HEADERS) $(DESTDIR)$(PREFIX)/include/lwcgl/
	install -d $(DESTDIR)$(PREFIX)/lib
	install -m 0644 $(LIB) $(DESTDIR)$(PREFIX)/lib/liblwcgl.a
	@echo "installed lwcgl to $(DESTDIR)$(PREFIX)"

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/include/lwcgl/lwcgl.h
	rm -f $(DESTDIR)$(PREFIX)/include/lwcgl/memory.h
	rm -f $(DESTDIR)$(PREFIX)/include/lwcgl/glfw.h
	rm -f $(DESTDIR)$(PREFIX)/include/lwcgl/opengl.h
	rmdir $(DESTDIR)$(PREFIX)/include/lwcgl 2>/dev/null || true
	rm -f $(DESTDIR)$(PREFIX)/lib/liblwcgl.a
	@echo "uninstalled lwcgl from $(DESTDIR)$(PREFIX)"

clean:
	rm -rf $(BUILD)
