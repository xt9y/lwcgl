CC ?= cc
CXX ?= c++
AR ?= ar
PREFIX ?= /usr/local

CPPFLAGS += -Iinclude -D_POSIX_C_SOURCE=200809L
CFLAGS ?= -O2
CFLAGS += -std=c11 -Wall -Wextra -Wpedantic

PKG_CFLAGS := $(shell pkg-config --cflags glfw3 2>/dev/null)
PKG_LIBS := $(shell pkg-config --libs glfw3 2>/dev/null)

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
CPPFLAGS += -DGL_SILENCE_DEPRECATION
PLATFORM_LIBS := -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
else
PLATFORM_LIBS := -lGL -lGLU -lm -ldl -lpthread
endif

CPPFLAGS += $(PKG_CFLAGS)
LIBS := $(PKG_LIBS) $(PLATFORM_LIBS)

BUILD := build
LIB := $(BUILD)/liblwcgl.a
OBJ := $(BUILD)/lwcgl.o $(BUILD)/display_ext.o $(BUILD)/gl11_compat.o $(BUILD)/glmodern.o

.PHONY: all clean install uninstall example check check-deps deps

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
	@printf '#include <GL/gl.h>\n' | $(CC) -x c -E - >/dev/null 2>&1 || { \
		echo "error: OpenGL development headers are missing"; \
		echo "run: make deps"; \
		exit 1; \
	}
	@printf '#include <GL/glu.h>\n' | $(CC) -x c -E - >/dev/null 2>&1 || { \
		echo "error: GLU development headers are missing"; \
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
		sudo apt-get install -y build-essential pkg-config libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev; \
	elif command -v pacman >/dev/null 2>&1; then \
		sudo pacman -S --needed base-devel pkgconf glfw-x11 mesa glu; \
	elif command -v dnf >/dev/null 2>&1; then \
		sudo dnf install -y gcc gcc-c++ make pkgconf-pkg-config glfw-devel mesa-libGL-devel mesa-libGLU-devel; \
	elif command -v zypper >/dev/null 2>&1; then \
		sudo zypper install -y gcc gcc-c++ make pkg-config libglfw3-devel Mesa-libGL-devel Mesa-libGLU-devel; \
	else \
		echo "error: unsupported package manager"; \
		echo "install GLFW 3, OpenGL, GLU, pkg-config and a C/C++ toolchain manually"; \
		exit 1; \
	fi
endif

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/lwcgl.o: src/lwcgl.c src/context_wrap.h include/lwcgl/lwcgl.h include/lwcgl/gl11_compat.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DLWCGL_CONTEXT_WRAP -include src/context_wrap.h -c src/lwcgl.c -o $@

$(BUILD)/display_ext.o: src/display_ext.c src/input_state.h include/lwcgl/lwcgl.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c src/display_ext.c -o $@

$(BUILD)/gl11_compat.o: src/gl11_compat.c include/lwcgl/lwcgl.h include/lwcgl/gl11_compat.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c src/gl11_compat.c -o $@

$(BUILD)/glmodern.o: src/glmodern.c include/lwcgl/lwcgl.h include/lwcgl/context.h include/lwcgl/glmodern.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c src/glmodern.c -o $@

$(LIB): $(OBJ)
	$(AR) rcs $@ $^

example: check-deps $(LIB)
	$(CXX) $(CPPFLAGS) -std=c++17 -Iinclude tests/rd132328_contract.cpp $(LIB) $(LIBS) -o $(BUILD)/rd132328-example

$(BUILD)/rd132328-contract: tests/rd132328_contract.cpp $(LIB)
	$(CXX) $(CPPFLAGS) -std=c++17 -Wall -Wextra -Wpedantic -Iinclude tests/rd132328_contract.cpp $(LIB) $(LIBS) -o $@

$(BUILD)/modern-gl-contract: tests/modern_gl_contract.cpp $(LIB)
	$(CXX) $(CPPFLAGS) -std=c++17 -Wall -Wextra -Wpedantic -Iinclude tests/modern_gl_contract.cpp $(LIB) $(LIBS) -o $@

$(BUILD)/display-update-contract: tests/display_update_contract.cpp $(LIB)
	$(CXX) $(CPPFLAGS) -std=c++17 -Wall -Wextra -Wpedantic -Werror -Iinclude tests/display_update_contract.cpp $(LIB) $(LIBS) -o $@

$(BUILD)/input-state-contract: tests/input_state_contract.cpp src/input_state.h | $(BUILD)
	$(CXX) $(CPPFLAGS) -std=c++17 -Wall -Wextra -Wpedantic -Werror tests/input_state_contract.cpp -o $@

check: check-deps $(BUILD)/rd132328-contract $(BUILD)/modern-gl-contract $(BUILD)/display-update-contract $(BUILD)/input-state-contract
	$(BUILD)/rd132328-contract
	$(BUILD)/modern-gl-contract
	$(BUILD)/display-update-contract
	$(BUILD)/input-state-contract

install: $(LIB)
	install -d $(DESTDIR)$(PREFIX)/include/lwcgl
	install -m 0644 include/lwcgl/lwcgl.h $(DESTDIR)$(PREFIX)/include/lwcgl/lwcgl.h
	install -m 0644 include/lwcgl/gl11_compat.h $(DESTDIR)$(PREFIX)/include/lwcgl/gl11_compat.h
	install -m 0644 include/lwcgl/context.h $(DESTDIR)$(PREFIX)/include/lwcgl/context.h
	install -m 0644 include/lwcgl/glmodern.h $(DESTDIR)$(PREFIX)/include/lwcgl/glmodern.h
	install -d $(DESTDIR)$(PREFIX)/lib
	install -m 0644 $(LIB) $(DESTDIR)$(PREFIX)/lib/liblwcgl.a

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/include/lwcgl/lwcgl.h
	rm -f $(DESTDIR)$(PREFIX)/include/lwcgl/gl11_compat.h
	rm -f $(DESTDIR)$(PREFIX)/include/lwcgl/context.h
	rm -f $(DESTDIR)$(PREFIX)/include/lwcgl/glmodern.h
	rmdir $(DESTDIR)$(PREFIX)/include/lwcgl 2>/dev/null || true
	rm -f $(DESTDIR)$(PREFIX)/lib/liblwcgl.a

clean:
	rm -rf $(BUILD)
