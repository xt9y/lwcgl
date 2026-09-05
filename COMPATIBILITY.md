# lwcgl v3.4.2 compatibility

`v3.4.2` is a native C ABI implementing a selected LWJGL 3.4.2-shaped GLFW/OpenGL surface; it does not claim the complete Java LWJGL distribution.

Requirements: GLFW >= 3.3, C11/C++17.

`<lwcgl/lwcgl.h>` is the canonical collision-safe API. Wrapper-owned OpenGL/GLFW constants and types use `LWCGL_` prefixes. `<lwcgl/compat_lwjgl3.h>` is optional and should only be used when unprefixed compatibility aliases are desired.

OpenGL capabilities are bound to the context identity and creation generation that produced them. Context switches clear stale thread-local dispatch. `GL.setCapabilities()` rejects capabilities belonging to another or destroyed context. Capability creation resolves into private state and publishes only after all functions required by the advertised core version are present, so a failed load cannot partially replace a working dispatch.

`GL.isFunctionAvailable()` queries the selected validated dispatch. Extension checks are separate through `GL.isExtensionSupported()`.

`make check` executes version/API, memory, native-header include-order, deterministic two-context dispatch/failure rollback, staged installed consumer, public-header standalone C/C++ compilation, example compilation, and real hidden-window clear/readback tests. `make sanitize` repeats the suite with ASan/UBSan.
