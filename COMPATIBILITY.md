# LWJGL 2.9.3 compatibility

The reference version is LWJGL **2.9.3**. The goal of lwcgl is native behavioral compatibility through a C ABI, with source-shaped singleton APIs for code that originally used LWJGL's static Java classes.

## Compatibility levels

- **Exact-shaped**: a C++ port can keep the LWJGL-looking call shape, for example `Display.create()` or `Keyboard.isKeyDown(Keyboard.KEY_W)`.
- **Native-equivalent**: the operation has the same native effect but Java-only syntax or object behavior cannot be represented literally by C.
- **Planned**: part of the 2.9.3 compatibility target but not implemented yet.

## Implemented core

| LWJGL 2.9.3 surface | Status | Native backend |
| --- | --- | --- |
| `DisplayMode(int,int)` | Exact-shaped for C++ via a C macro | C aggregate |
| `Display` basic lifecycle | Exact-shaped | GLFW |
| `Display.update/processMessages` | Exact-shaped | GLFW |
| `Display` mode enumeration | Native-equivalent | GLFW monitor modes |
| `Display` fullscreen/windowed | Exact-shaped | GLFW |
| `Display` title/location/resizable/vsync | Exact-shaped | GLFW |
| `Keyboard` full LWJGL2 numeric key-code namespace | Exact-shaped | GLFW mapping |
| `Keyboard.isKeyDown` | Exact-shaped | GLFW polling |
| `Keyboard.next` event queue | Exact-shaped | GLFW callbacks |
| `Keyboard` repeat events | Exact-shaped | GLFW callbacks |
| `Mouse` lifecycle/grab | Exact-shaped | GLFW |
| `Mouse` absolute + relative motion | Exact-shaped | GLFW callbacks |
| `Mouse.next` event queue | Exact-shaped | GLFW callbacks |
| `Mouse` wheel/buttons | Exact-shaped | GLFW callbacks |
| `BufferUtils` native buffers | Exact-shaped factory API | C heap |
| Buffer clear/flip/rewind/position/limit | Native-equivalent | C heap |
| GL11 fixed-function functions already present in C OpenGL | Native-equivalent/direct | system OpenGL |
| GL11 NIO-buffer overloads used by RubyDung | Exact-shaped macros/helpers | system OpenGL |
| GLU perspective | direct native call | system GLU |
| GLU NIO-buffer overloads used by RubyDung | Exact-shaped macros/helpers | system GLU |
| `Sys` version/timer/alert basics | Exact-shaped | libc/platform timer |

The compatibility path intentionally supports the old renderer rather than translating it to modern OpenGL. That includes compatibility-context functionality such as fixed-function matrices, fog, selection mode, client-side arrays, quads and GLU.

Examples that remain source-shaped in C++:

```cpp
Display.setDisplayMode(new DisplayMode(1024, 768));
Display.create();
Keyboard.create();
Mouse.create();
Mouse.setGrabbed(true);

if (Keyboard.isKeyDown(Keyboard.KEY_W)) {
    // ...
}

while (Mouse.next()) {
    int button = Mouse.getEventButton();
}
```

The legacy GL buffer overload compatibility includes:

```cpp
glFog(GL_FOG_COLOR, fogColor);
glGetInteger(GL_VIEWPORT, viewportBuffer);
glSelectBuffer(selectBuffer);
GLuint texture = glGenTextures();
glVertexPointer(3, 0, vertexBuffer);
glTexCoordPointer(2, 0, textureCoordinateBuffer);
glColorPointer(3, 0, colorBuffer);
gluPickMatrix(x, y, 5.0, 5.0, viewportBuffer);
```

## Full 2.9.3 target still to implement

The following are part of the project target and are not yet complete:

- all remaining `org.lwjgl` utility/runtime classes
- `Cursor`, `Controllers`, and controller enumeration
- complete `Display` PixelFormat / ContextAttribs overload family
- OpenGL 1.1 through the full 2.9.3 generated OpenGL binding surface and extension classes
- OpenAL 1.0/1.1 and extension bindings
- OpenCL binding surface
- OpenGL ES / EGL binding surface
- remaining `lwjgl_util` GLU, vector, geometry, shader, mapped-object and audio helpers
- platform-specific cursor, clipboard and adapter details

## Irreducible Java-to-C differences

A pure C ABI cannot literally reproduce Java language/runtime features. These differences are architectural rather than missing native functionality:

1. **Method overloading.** C has one symbol/signature per function pointer. Where practical, lwcgl uses macros or explicit suffixed entry points to preserve the common source shape.
2. **Instance methods with implicit `this`.** C buffer operations require the buffer pointer to be supplied to the native operation; there is no hidden object receiver.
3. **Exceptions.** C entry points return error codes and `lwcglGetLastError()` instead of throwing `LWJGLException`.
4. **Garbage collection / Java object ownership.** Native allocations have explicit ownership. The literal C++ expression `new DisplayMode(...)` is accepted for source similarity, but a C implementation cannot legally `delete` a C++ allocation; callers wanting strict ownership should use a stack `DisplayMode` and pass its address.
5. **AWT/Applet integration, reflection and Java callbacks.** These require platform-native equivalents rather than literal Java object compatibility.

These limitations do not require a JVM and do not change the legacy OpenGL rendering path used by `rd-132328`.
