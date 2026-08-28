#!/bin/sh
set -eu

PREFIX=${PREFIX:-/usr/local}
DESTDIR=${DESTDIR:-}
PROFILE=${PROFILE:-debug}
LIB="build/$PROFILE/liblwcgl.a"

if [ ! -f "$LIB" ]; then
    echo "error: $LIB does not exist" >&2
    if [ "$PROFILE" = "release" ]; then
        echo "run: c build --release" >&2
    else
        echo "run: c build" >&2
    fi
    exit 1
fi

install -d "$DESTDIR$PREFIX/include/lwcgl"
install -m 0644 include/lwcgl/lwcgl.h "$DESTDIR$PREFIX/include/lwcgl/lwcgl.h"
install -m 0644 include/lwcgl/memory.h "$DESTDIR$PREFIX/include/lwcgl/memory.h"
install -m 0644 include/lwcgl/glfw.h "$DESTDIR$PREFIX/include/lwcgl/glfw.h"
install -m 0644 include/lwcgl/opengl.h "$DESTDIR$PREFIX/include/lwcgl/opengl.h"
install -d "$DESTDIR$PREFIX/lib"
install -m 0644 "$LIB" "$DESTDIR$PREFIX/lib/liblwcgl.a"

echo "installed lwcgl to $DESTDIR$PREFIX"
