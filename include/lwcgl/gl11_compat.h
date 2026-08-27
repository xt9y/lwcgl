#ifndef LWCGL_GL11_COMPAT_H
#define LWCGL_GL11_COMPAT_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* LWJGL GL11 overloads that do not map 1:1 to the C OpenGL ABI. */
void lwcgl_glFog(GLenum pname, const LWCGLBuffer *params);
int lwcgl_glGetIntegerValue(GLenum pname);
void lwcgl_glGetIntegerBuffer(GLenum pname, IntBuffer *params);
void lwcgl_glSelectBuffer(IntBuffer *buffer);
GLuint lwcgl_glGenTexture(void);
void lwcgl_glGenTexturesBuffer(IntBuffer *textures);
void lwcgl_glDeleteTexture(GLuint texture);
void lwcgl_glVertexPointer3(GLint size, GLsizei stride, const LWCGLBuffer *pointer);
void lwcgl_glTexCoordPointer3(GLint size, GLsizei stride, const LWCGLBuffer *pointer);
void lwcgl_glColorPointer3(GLint size, GLsizei stride, const LWCGLBuffer *pointer);
void lwcgl_glLoadMatrixBuffer(const LWCGLBuffer *matrix);
void lwcgl_glMultMatrixBuffer(const LWCGLBuffer *matrix);

/* LWJGL GLU overloads using NIO buffers. */
void lwcgl_gluPickMatrix(GLdouble x, GLdouble y, GLdouble width, GLdouble height,
                         const IntBuffer *viewport);
GLint lwcgl_gluBuild2DMipmaps(GLenum target, GLint components, GLsizei width,
                              GLsizei height, GLenum format, GLenum type,
                              const ByteBuffer *data);

#ifdef __cplusplus
}
#endif

/*
 * These compatibility macros intentionally prefer the LWJGL 2 calling shape.
 * Define LWCGL_IMPLEMENTATION before including lwcgl.h inside the library
 * implementation to keep access to the native C OpenGL symbols.
 */
#ifndef LWCGL_IMPLEMENTATION

#define glFog(pname_, params_) \
    lwcgl_glFog((pname_), (const LWCGLBuffer *)(params_))

#define LWCGL_GETINTEGER_SELECT(_1, _2, NAME, ...) NAME
#define glGetInteger(...) \
    LWCGL_GETINTEGER_SELECT(__VA_ARGS__, lwcgl_glGetIntegerBuffer, lwcgl_glGetIntegerValue)(__VA_ARGS__)

#define glSelectBuffer(buffer_) \
    lwcgl_glSelectBuffer((buffer_))

/* RubyDung uses the no-argument LWJGL overload. */
#define glGenTextures() \
    lwcgl_glGenTexture()

#define glDeleteTextures(texture_) \
    lwcgl_glDeleteTexture((GLuint)(texture_))

#define glVertexPointer(size_, stride_, pointer_) \
    lwcgl_glVertexPointer3((size_), (stride_), (const LWCGLBuffer *)(pointer_))

#define glTexCoordPointer(size_, stride_, pointer_) \
    lwcgl_glTexCoordPointer3((size_), (stride_), (const LWCGLBuffer *)(pointer_))

#define glColorPointer(size_, stride_, pointer_) \
    lwcgl_glColorPointer3((size_), (stride_), (const LWCGLBuffer *)(pointer_))

#define glLoadMatrix(matrix_) \
    lwcgl_glLoadMatrixBuffer((const LWCGLBuffer *)(matrix_))

#define glMultMatrix(matrix_) \
    lwcgl_glMultMatrixBuffer((const LWCGLBuffer *)(matrix_))

#define gluPickMatrix(x_, y_, width_, height_, viewport_) \
    lwcgl_gluPickMatrix((x_), (y_), (width_), (height_), (viewport_))

#define gluBuild2DMipmaps(target_, components_, width_, height_, format_, type_, data_) \
    lwcgl_gluBuild2DMipmaps((target_), (components_), (width_), (height_), \
                            (format_), (type_), (data_))

#endif

#endif
