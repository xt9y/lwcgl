#!/bin/sh
set -eu

PREFIX=${PREFIX:-/usr/local}
DESTDIR=${DESTDIR:-}

rm -f "$DESTDIR$PREFIX/include/lwcgl/lwcgl.h"
rm -f "$DESTDIR$PREFIX/include/lwcgl/memory.h"
rm -f "$DESTDIR$PREFIX/include/lwcgl/glfw.h"
rm -f "$DESTDIR$PREFIX/include/lwcgl/opengl.h"
rmdir "$DESTDIR$PREFIX/include/lwcgl" 2>/dev/null || true
rm -f "$DESTDIR$PREFIX/lib/liblwcgl.a"

echo "uninstalled lwcgl from $DESTDIR$PREFIX"
