# lwcgl

## Current version

```text
lwcgl v3.4.2
upstream compatibility target: LWJGL 3.4.2
```

There is no JVM and no Java jar involved. LWJGL 3 is primarily a Java binding layer over native APIs; in C the equivalent is a thin native ABI over those same libraries.

## Minimal LWJGL 3-style program

```c
#include <lwcgl/lwcgl.h>

int main(void) {
    if (!GLFW.glfwInit())
        return 1;

    GLFW.glfwWindowHint(GLFW.GLFW_CONTEXT_VERSION_MAJOR, 3);
    GLFW.glfwWindowHint(GLFW.GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFW.glfwWindowHint(GLFW.GLFW_OPENGL_PROFILE, GLFW.GLFW_OPENGL_CORE_PROFILE);

    LWCGLFWwindow *window = GLFW.glfwCreateWindow(1280, 720, "lwcgl", NULL, NULL);
    if (!window) {
        GLFW.glfwTerminate();
        return 1;
    }

    GLFW.glfwMakeContextCurrent(window);
    GLCapabilities *caps = GL.createCapabilities();
    if (!caps) {
        GLFW.glfwDestroyWindow(window);
        GLFW.glfwTerminate();
        return 1;
    }

    while (!GLFW.glfwWindowShouldClose(window)) {
        GL11.glClearColor(0.08f, 0.08f, 0.10f, 1.0f);
        GL11.glClear((GLbitfield)GL11.GL_COLOR_BUFFER_BIT);
        GLFW.glfwSwapBuffers(window);
        GLFW.glfwPollEvents();
    }

    GL.destroyCapabilities(caps);
    GLFW.glfwDestroyWindow(window);
    GLFW.glfwTerminate();
    return 0;
}
```

## Memory APIs

LWJGL 3 code commonly uses `MemoryUtil` and `MemoryStack`. lwcgl provides native equivalents:

```c
void *heap = MemoryUtil.memAlloc(4096);
MemoryUtil.memFree(heap);

LWCGLMemoryStack *stack = MemoryStack.stackPush();
char *name = MemoryStack.UTF8(stack, "shader");
/* use name */
MemoryStack.stackPop(stack);
```

`MemoryStack` is thread-local and enforces LIFO frame destruction.

## Build

Required dependencies:

- GLFW 3 development package
- `pkg-config`
- Make
- C11/C++17 toolchain

On supported systems, dependencies can be installed with:

```sh
make deps
```

Build and install to `/usr/local`:

```sh
make
sudo make install
```

## Scope

LWJGL 3 ships bindings for many optional third-party libraries in addition to its core runtime. `lwcgl` v3.4.2 implements the central C-facing runtime surface first: GLFW, OpenGL and native memory. Optional upstream binding families such as Vulkan, OpenAL, OpenCL, stb, Assimp, bgfx, Nuklear, OpenXR and others are tracked separately in `COMPATIBILITY.md`; native C applications can already use those libraries directly alongside lwcgl.
