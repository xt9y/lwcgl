# lwcgl

`lwcgl` is a native C compatibility implementation of the LWJGL 2.9.3 programming model, intended to let C and C++ ports of old LWJGL 2 software keep the original low-level architecture instead of being rewritten around a modern engine.

The project targets the public behavior of LWJGL 2.9.3 while exposing a C ABI. C++ can consume the C header directly; there is no C++ facade layer.

The first compatibility target is Minecraft `rd-132328` / RubyDung. Its common LWJGL calls are intentionally source-shaped like the original:

```cpp
#include <lwcgl/lwcgl.h>

int main() {
    Display.setDisplayMode(new DisplayMode(1024, 768));
    Display.create();
    Keyboard.create();
    Mouse.create();

    Mouse.setGrabbed(true);

    while (!Keyboard.isKeyDown(Keyboard.KEY_ESCAPE) &&
           !Display.isCloseRequested()) {
        Display.update();
    }

    Mouse.destroy();
    Keyboard.destroy();
    Display.destroy();
}
```

`DisplayMode(...)` is a function-like compatibility macro defined by the C header. In C++ it expands to aggregate initialization, so `new DisplayMode(1024, 768)` remains valid without introducing a C++ wrapper.

Docs (Thanks to AI): https://xt9y.de/lwcgl.html

## Architecture

```text
C / C++ program
      |
      v
lwcgl public C ABI
      |
      +-- Display / Keyboard / Mouse -> GLFW
      +-- legacy OpenGL              -> system OpenGL compatibility context
      +-- GLU                        -> system GLU
      +-- buffers                    -> native C memory
```

The library does not embed a JVM and does not call the Java LWJGL jar. It reimplements the LWJGL 2.9.3-facing behavior natively.

## Build

Linux development packages currently required:

- GLFW 3
- OpenGL development headers/library
- GLU development headers/library

Build and install:

```sh
make
sudo make install
```

The default install prefix is `/usr/local`.

## Compatibility policy

The compatibility reference is LWJGL 2.9.3, including its legacy `org.lwjgl`, `org.lwjgl.input`, `org.lwjgl.opengl`, `org.lwjgl.openal`, `org.lwjgl.opencl`, `org.lwjgl.opengles`, and `lwjgl_util` surfaces.

Java-only runtime concepts cannot be literally source-identical in a pure C ABI (Java method overloading, garbage collection, exceptions, reflection, AWT/Applet integration and Java object identity). Where exact syntax is impossible, lwcgl provides the closest deterministic native equivalent and records the deviation in `COMPATIBILITY.md`.

The `rd-132328` compatibility surface is treated as strict: Display, DisplayMode, Keyboard, Mouse, native-style buffers, legacy OpenGL/GLU behavior, LWJGL 2 key codes and input event semantics are implemented without introducing a C++ facade.
