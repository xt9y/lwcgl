CC ?= cc
AR ?= ar
PREFIX ?= /usr/local

CPPFLAGS += -Iinclude
CFLAGS ?= -O2
CFLAGS += -std=c11 -Wall -Wextra -Wpedantic

PKG_CFLAGS := $(shell pkg-config --cflags glfw3 2>/dev/null)
PKG_LIBS := $(shell pkg-config --libs glfw3 2>/dev/null)

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
PLATFORM_LIBS := -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
else
PLATFORM_LIBS := -lGL -lGLU -lm -ldl -lpthread
endif

CPPFLAGS += $(PKG_CFLAGS)
LIBS := $(PKG_LIBS) $(PLATFORM_LIBS)

BUILD := build
LIB := $(BUILD)/liblwcgl.a
OBJ := $(BUILD)/lwcgl.o

.PHONY: all clean install uninstall example

all: $(LIB)

$(BUILD):
	mkdir -p $(BUILD)

$(OBJ): src/lwcgl.c include/lwcgl/lwcgl.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c src/lwcgl.c -o $@

$(LIB): $(OBJ)
	$(AR) rcs $@ $^

example: $(LIB)
	$(CXX) -std=c++17 -Iinclude examples/rd132328.cpp $(LIB) $(LIBS) -o $(BUILD)/rd132328-example

install: $(LIB)
	install -d $(DESTDIR)$(PREFIX)/include/lwcgl
	install -m 0644 include/lwcgl/lwcgl.h $(DESTDIR)$(PREFIX)/include/lwcgl/lwcgl.h
	install -d $(DESTDIR)$(PREFIX)/lib
	install -m 0644 $(LIB) $(DESTDIR)$(PREFIX)/lib/liblwcgl.a

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/include/lwcgl/lwcgl.h
	rmdir $(DESTDIR)$(PREFIX)/include/lwcgl 2>/dev/null || true
	rm -f $(DESTDIR)$(PREFIX)/lib/liblwcgl.a

clean:
	rm -rf $(BUILD)
