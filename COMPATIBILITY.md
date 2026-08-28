# lwcgl v3.4.2 compatibility

## Version target

`main` targets the programming model of **LWJGL 3.4.2**. The previous LWJGL 2.9.3 implementation is preserved unchanged on `v2.9.3`.

This version is intentionally not source-compatible with the old `Display`, `Keyboard`, `Mouse`, `BufferUtils` and fixed-function RubyDung facade on `main`, because those are LWJGL 2 concepts. Use `v2.9.3` for that code.

## Compatibility principle

LWJGL 3 is a Java native-binding toolkit. In Java, generated classes wrap C APIs and manage native addresses, callbacks and off-heap memory. In a native C library, the closest equivalent is:

1. preserve the LWJGL module/class organization where it remains useful (`GLFW`, `GL`, `GL11` ... `GL46`, `MemoryUtil`, `MemoryStack`);
2. expose native C handles and callback signatures directly;
3. resolve OpenGL functions from the active context like LWJGL's `GL.createCapabilities()`;
4. avoid inventing Java-only behavior where C already provides the native primitive.

## Implemented on main

### Core/version

- compile-time `LWCGL_VERSION_*` = 3.4.2
- runtime `LWCGL.getVersion*()`
- thread-local last-error string for lwcgl-owned failures

### `org.lwjgl.glfw.GLFW` model

Implemented C table: `GLFW`.

Covered areas:

- initialization / termination / version
- error callback
- primary monitor and monitor callback
- window hints and window lifecycle
- context creation/current-context handling
- swap interval and buffer swap
- event polling/waiting/posting
- window size, position, framebuffer size, content scale and opacity
- iconify/restore/maximize/show/hide/focus/attention
- window attributes and user pointers
- window callbacks
- keyboard, mouse and cursor-position input
- input modes including raw mouse mode
- input callbacks
- joystick discovery/state/name/GUID/user pointer
- gamepad mapping/name checks
- clipboard
- timer
- OpenGL extension/procedure lookup

The public header intentionally defines opaque native handle aliases (`LWCGLFWwindow`, `LWCGLFWmonitor`) instead of exposing Java `long` handles.

### `org.lwjgl.opengl.GL`

Implemented C table: `GL`.

- `GL.createCapabilities()` requires a current GLFW OpenGL context
- dynamically resolves entry points with `glfwGetProcAddress`
- parses the context version
- populates `GLCapabilities.OpenGL11` through `OpenGL46`
- `GL.getCapabilities()` / `GL.setCapabilities()` are thread-local
- explicit `GL.destroyCapabilities()` is required because C has no GC
- arbitrary function lookup via `GL.getFunctionAddress()` / `GL.isFunctionAvailable()`

### OpenGL class progression

Function tables and representative constants exist for:

| LWJGL class | Native coverage |
| --- | --- |
| `GL11` | core state, textures, draw, query/readback |
| `GL12` | range draws, 3D textures |
| `GL13` | active texture, multisample, compressed texture upload |
| `GL14` | blend equation/separate blend, point parameters |
| `GL15` | buffers, mapping, occlusion/query objects |
| `GL20` | shaders, programs, uniforms, vertex attributes |
| `GL21` | non-square matrix uniforms |
| `GL30` | VAO, FBO/RBO, mapped ranges, mipmaps, blit/MSAA storage |
| `GL31` | instancing, copy buffers, uniform blocks |
| `GL32` | sync objects and base-vertex drawing |
| `GL33` | attribute divisors, samplers, timer queries |
| `GL40` | indirect draws, tessellation patch state, indexed blending |
| `GL41` | program pipelines and program uniforms |
| `GL42` | barriers, immutable texture storage, image binding |
| `GL43` | compute, debug output, object labels |
| `GL44` | buffer storage, multi-bind, clear texture image |
| `GL45` | direct-state-access subset, clip control |
| `GL46` | SPIR-V specialization, indirect-count draws |

The tables are loaded even when a context is older. Callers must check the corresponding `GLCapabilities` flag and/or function pointer before using a newer entry point.

### `org.lwjgl.system.MemoryUtil`

Implemented table: `MemoryUtil`.

- `memAlloc`
- `memCalloc`
- `memRealloc`
- `memFree`
- pointer/address conversion
- native ASCII/UTF-8 null-terminated copies
- null-terminated byte length

### `org.lwjgl.system.MemoryStack`

Implemented table: `MemoryStack`.

- thread-local stack frames
- `stackPush()` / capacity override
- LIFO `stackPop()`
- aligned frame allocation
- zeroed allocation
- scoped ASCII/UTF-8 copies
- pointer/capacity introspection

Java's `try (MemoryStack stack = stackPush())` maps to explicit `stackPush` / `stackPop` in C.

## Native-language differences

These are deliberate differences, not missing emulation:

- Java classes become C API tables.
- Java `long` native handles become typed C pointers where possible.
- Java NIO buffers are not recreated; native pointers and C allocations are used.
- Java exceptions become return values plus `lwcglGetLastError()` for lwcgl-owned operations.
- Java GC/AutoCloseable cleanup becomes explicit C cleanup.
- Java overloads are represented by native C signatures instead of artificial overload dispatch.
- Java callback objects become native function pointers.

## Optional LWJGL binding families

LWJGL 3.4.2 contains many generated bindings beyond the core surface above. They are not all re-wrapped as lwcgl tables yet. This includes, among others:

- Vulkan / shaderc / SPIR-V tooling
- OpenAL
- OpenCL
- stb
- Assimp
- bgfx
- Nuklear
- OpenXR
- FreeType / HarfBuzz
- meshoptimizer
- KTX
- LLVM
- libffi
- LMDB
- mimalloc / jemalloc
- SDL 3
- RenderDoc

In C/C++, these libraries already expose their canonical native APIs, so applications can link them directly while using lwcgl for GLFW/OpenGL/memory. Future lwcgl modules should only add value where a consistent LWJGL-shaped table, loader or compatibility behavior is useful.

## CI contracts

`make check` verifies:

- v3.4.2 compile-time and runtime version identity
- C compilation of the GLFW + capabilities + modern buffer/shader/VAO surface
- C++17 consumption of the same C ABI
- Linux and macOS builds
