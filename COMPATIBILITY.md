# Compatibility

Branch `v2.9.3` targets the LWJGL 2.9.3 programming model through a native C ABI.

Verified surfaces are covered by executable contracts in `tests/`: native buffers, Display lifecycle, Keyboard/Mouse lifecycle, legacy GL/GLU buffer overloads, progressive modern OpenGL loading, header include order, and staged C/C++ consumers.

Java-only behavior such as JVM object identity, exceptions, reflection, AWT integration, and garbage collection is outside the native ABI. LWJGL-shaped GL11 macro syntax is opt-in via `LWCGL_ENABLE_LWJGL2_COMPAT` or `<lwcgl/lwjgl2_compat.h>`; the canonical `<lwcgl/lwcgl.h>` header does not redefine OpenGL function names.

Supported build hosts: Linux and macOS. GLFW 3.3 or newer is required.
