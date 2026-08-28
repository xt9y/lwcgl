# lwcgl

`lwcgl` is a native C/C++ implementation of the programming model exposed by LWJGL. The active `main` line tracks **LWJGL 3.4.2** and exposes a C ABI shaped around the modern LWJGL 3 modules instead of the removed LWJGL 2 `Display` / `Keyboard` / `Mouse` API.

The previous LWJGL 2.9.3 compatibility implementation is preserved on the [`v2.9.3`](../../tree/v2.9.3) branch.

## Current version

```text
lwcgl v3.4.2
upstream compatibility target: LWJGL 3.4.2
```

The version is available both at compile time and runtime:

```c
#include <lwcgl/lwcgl.h>

printf("%s\n", LWCGL_VERSION_NAME);       // v3.4.2
printf("%s\n", LWCGL.getVersion());      // 3.4.2
```

## What changed from v2.9.3

LWJGL 3 is architecturally different from LWJGL 2. The old global `Display`, `Keyboard` and `Mouse` classes are not part of modern LWJGL. The current branch therefore exposes the modern model:

```text
C / C++ program
      |
      v
lwcgl v3.4.2 C ABI
      |
      +-- GLFW.*            -> windowing, input, events and contexts
      +-- GL.*              -> OpenGL capability discovery / proc loading
      +-- GL11 ... GL46     -> OpenGL 1.1 through 4.6 function tables
      +-- MemoryUtil.*      -> native heap allocation helpers
      +-- MemoryStack.*     -> thread-local scoped native allocations
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

## Modern OpenGL coverage

`GL.createCapabilities()` resolves OpenGL entry points from the current GLFW context. The public tables currently cover the main core progression through OpenGL 4.6:

- `GL11` — state, textures, drawing, readback
- `GL12`–`GL14` — 3D textures, multitexture, blending additions
- `GL15` — vertex/index buffers and queries
- `GL20`–`GL21` — shaders, programs, uniforms, vertex attributes
- `GL30`–`GL33` — VAOs, FBOs, mapping, instancing, samplers, sync/query features
- `GL40`–`GL42` — tessellation-era, pipelines, texture storage, image binding and barriers
- `GL43`–`GL44` — compute, debug output, SSBO-era APIs, persistent storage and multi-bind
- `GL45` — direct-state-access helpers and clip control
- `GL46` — SPIR-V specialization and indirect-count draw entry points

Capability flags (`OpenGL11` ... `OpenGL46`) are derived from the actual current context version, not from the build machine.

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

Required dependency:

- GLFW 3 development package
- `pkg-config`
- C11 compiler

Build and run the compile/runtime contracts:

```sh
make check
```

Install:

```sh
sudo make install
```

The default prefix is `/usr/local`.

## Branch policy

- `v2.9.3` — frozen LWJGL 2.9.3 / RubyDung compatibility line
- `main` — current LWJGL-compatible implementation; currently `v3.4.2`

Do not port LWJGL 2 projects by replacing only the include path. Their window/input code should either stay on `v2.9.3` or be migrated to the GLFW model used by LWJGL 3.

## Scope

LWJGL 3 ships bindings for many optional third-party libraries in addition to its core runtime. `lwcgl` v3.4.2 implements the central C-facing runtime surface first: GLFW, OpenGL and native memory. Optional upstream binding families such as Vulkan, OpenAL, OpenCL, stb, Assimp, bgfx, Nuklear, OpenXR and others are tracked separately in `COMPATIBILITY.md`; native C applications can already use those libraries directly alongside lwcgl.
