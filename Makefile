CC ?= cc
CXX ?= c++
AR ?= ar
PKG_CONFIG ?= pkg-config
PREFIX ?= /usr/local
DESTDIR ?=
VERSION := 3.4.2
BUILD := build
LIB := $(BUILD)/liblwcgl-$(VERSION).a
PUBLIC_HEADERS := $(wildcard include/lwcgl/*.h)
SRC := $(wildcard src/*.c)
OBJ := $(patsubst src/%.c,$(BUILD)/%.o,$(SRC))
CONFIG := $(BUILD)/.build-config
PC := $(BUILD)/lwcgl-$(VERSION).pc
TEST_DIR := $(BUILD)/tests
CPPFLAGS += -Iinclude -D_POSIX_C_SOURCE=200809L
CFLAGS ?= -O2
CFLAGS += -std=c11 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -O2
CXXFLAGS += -std=c++17 -Wall -Wextra -Wpedantic
LDFLAGS ?=
LDLIBS ?=
PKG_CFLAGS := $(shell $(PKG_CONFIG) --cflags glfw3 2>/dev/null)
PKG_LIBS := $(shell $(PKG_CONFIG) --libs glfw3 2>/dev/null)
CPPFLAGS += $(PKG_CFLAGS)
LIBS := $(PKG_LIBS) $(LDLIBS)
TEST_BINS := $(TEST_DIR)/version $(TEST_DIR)/memory $(TEST_DIR)/compat $(TEST_DIR)/native-before $(TEST_DIR)/native-after $(TEST_DIR)/runtime $(TEST_DIR)/context
.PHONY: all clean check test sanitize install uninstall check-deps deps stage-check example header-check FORCE
all: check-deps $(LIB)
test: check
check-deps:
	@command -v $(PKG_CONFIG) >/dev/null 2>&1 || { echo "error: pkg-config is required"; exit 1; }
	@$(PKG_CONFIG) --atleast-version=3.3 glfw3 || { echo "error: GLFW >= 3.3 development files are required"; exit 1; }
deps:
	@set -e; if command -v brew >/dev/null 2>&1; then HOMEBREW_NO_AUTO_UPDATE=1 brew install pkg-config glfw; elif command -v apt-get >/dev/null 2>&1; then sudo apt-get update && sudo apt-get install -y build-essential pkg-config libglfw3-dev xvfb; elif command -v pacman >/dev/null 2>&1; then sudo pacman -S --needed base-devel pkgconf glfw-x11 xorg-server-xvfb; elif command -v dnf >/dev/null 2>&1; then sudo dnf install -y gcc gcc-c++ make pkgconf-pkg-config glfw-devel xorg-x11-server-Xvfb; else echo "error: install GLFW >= 3.3 and a C/C++ toolchain"; exit 1; fi
$(BUILD) $(TEST_DIR):
	mkdir -p $@
FORCE:
$(CONFIG): FORCE | $(BUILD)
	@{ printf '%s\n' 'CC=$(CC)' 'CXX=$(CXX)' 'AR=$(AR)' 'CPPFLAGS=$(CPPFLAGS)' 'CFLAGS=$(CFLAGS)' 'CXXFLAGS=$(CXXFLAGS)' 'LDFLAGS=$(LDFLAGS)' 'LDLIBS=$(LDLIBS)' 'SRC=$(SRC)' 'HEADERS=$(PUBLIC_HEADERS)'; } > $@.tmp
	@cmp -s $@.tmp $@ 2>/dev/null || mv $@.tmp $@
	@rm -f $@.tmp
$(BUILD)/%.o: src/%.c $(PUBLIC_HEADERS) src/lwcgl_internal.h $(CONFIG) | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -Werror -c $< -o $@
$(LIB): $(OBJ)
	rm -f $@
	$(AR) rcs $@ $^
$(PC): pkgconfig/lwcgl-$(VERSION).pc.in | $(BUILD)
	sed 's|@PREFIX@|$(PREFIX)|g' $< > $@
$(TEST_DIR)/version: tests/version_contract.c $(LIB) | $(TEST_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -Werror $< $(LIB) $(LDFLAGS) $(LIBS) -o $@
$(TEST_DIR)/memory: tests/memory_contract.c $(LIB) | $(TEST_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -Werror $< $(LIB) $(LDFLAGS) $(LIBS) -o $@
$(TEST_DIR)/compat: tests/compat_contract.cpp $(LIB) | $(TEST_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -Werror $< $(LIB) $(LDFLAGS) $(LIBS) -o $@
$(TEST_DIR)/native-before: tests/header_native_before.c $(LIB) | $(TEST_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -Werror $< $(LIB) $(LDFLAGS) $(LIBS) -o $@
$(TEST_DIR)/native-after: tests/header_native_after.c $(LIB) | $(TEST_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -Werror $< $(LIB) $(LDFLAGS) $(LIBS) -o $@
$(TEST_DIR)/runtime: tests/runtime_gl.c $(LIB) | $(TEST_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -Werror $< $(LIB) $(LDFLAGS) $(LIBS) -o $@
$(TEST_DIR)/context: tests/context_contract.c tests/fake_runtime.c src/lwcgl.c src/opengl.c include/lwcgl/*.h src/lwcgl_internal.h | $(TEST_DIR)
	$(CC) -Iinclude -Itests/fake_include -D_POSIX_C_SOURCE=200809L $(CFLAGS) -Werror tests/context_contract.c tests/fake_runtime.c src/lwcgl.c src/opengl.c $(LDFLAGS) -o $@
header-check: | $(TEST_DIR)
	@set -e; for h in $(PUBLIC_HEADERS); do n=$$(basename $$h .h); printf '#include <lwcgl/%s.h>\nint main(void){return 0;}\n' $$n > $(TEST_DIR)/header-$$n.c; $(CC) -Iinclude $(CFLAGS) -Werror -c $(TEST_DIR)/header-$$n.c -o $(TEST_DIR)/header-$$n-c.o; printf '#include <lwcgl/%s.h>\nint main(){return 0;}\n' $$n > $(TEST_DIR)/header-$$n.cpp; $(CXX) -Iinclude $(CXXFLAGS) -Werror -c $(TEST_DIR)/header-$$n.cpp -o $(TEST_DIR)/header-$$n-cpp.o; done
check: check-deps $(TEST_BINS) stage-check header-check example
	@set -e; for t in $(TEST_BINS); do echo RUN $$t; $$t; done
sanitize:
	$(MAKE) clean
	$(MAKE) CFLAGS='-O1 -g -std=c11 -Wall -Wextra -Wpedantic -fsanitize=address,undefined -fno-omit-frame-pointer' CXXFLAGS='-O1 -g -std=c++17 -Wall -Wextra -Wpedantic -fsanitize=address,undefined -fno-omit-frame-pointer' LDFLAGS='-fsanitize=address,undefined' check
stage-check: $(LIB) $(PC)
	rm -rf $(BUILD)/stage $(BUILD)/stage-prefix
	$(MAKE) install DESTDIR=$(abspath $(BUILD)/stage) PREFIX=/usr
	test -f $(BUILD)/stage/usr/lib/liblwcgl-$(VERSION).a
	test -f $(BUILD)/stage/usr/include/lwcgl-$(VERSION)/lwcgl/lwcgl.h
	$(MAKE) install PREFIX=$(abspath $(BUILD)/stage-prefix)
	PKG_CONFIG_PATH=$(abspath $(BUILD)/stage-prefix)/lib/pkgconfig $(CC) $(CFLAGS) tests/consumer.c $$(PKG_CONFIG_PATH=$(abspath $(BUILD)/stage-prefix)/lib/pkgconfig $(PKG_CONFIG) --cflags --libs --static lwcgl-$(VERSION)) -o $(TEST_DIR)/consumer-c
	PKG_CONFIG_PATH=$(abspath $(BUILD)/stage-prefix)/lib/pkgconfig $(CXX) $(CXXFLAGS) tests/consumer.cpp $$(PKG_CONFIG_PATH=$(abspath $(BUILD)/stage-prefix)/lib/pkgconfig $(PKG_CONFIG) --cflags --libs --static lwcgl-$(VERSION)) -o $(TEST_DIR)/consumer-cpp
	$(TEST_DIR)/consumer-c
	$(TEST_DIR)/consumer-cpp
install: check-deps $(LIB) $(PC)
	install -d $(DESTDIR)$(PREFIX)/include/lwcgl-$(VERSION)/lwcgl $(DESTDIR)$(PREFIX)/lib/pkgconfig
	install -m 0644 $(PUBLIC_HEADERS) $(DESTDIR)$(PREFIX)/include/lwcgl-$(VERSION)/lwcgl/
	install -m 0644 $(LIB) $(DESTDIR)$(PREFIX)/lib/liblwcgl-$(VERSION).a
	sed 's|@PREFIX@|$(PREFIX)|g' pkgconfig/lwcgl-$(VERSION).pc.in > $(DESTDIR)$(PREFIX)/lib/pkgconfig/lwcgl-$(VERSION).pc
uninstall:
	rm -rf $(DESTDIR)$(PREFIX)/include/lwcgl-$(VERSION)
	rm -f $(DESTDIR)$(PREFIX)/lib/liblwcgl-$(VERSION).a $(DESTDIR)$(PREFIX)/lib/pkgconfig/lwcgl-$(VERSION).pc
example: check-deps $(LIB) | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -Werror examples/clear.c $(LIB) $(LDFLAGS) $(LIBS) -o $(BUILD)/clear
clean:
	rm -rf $(BUILD)
