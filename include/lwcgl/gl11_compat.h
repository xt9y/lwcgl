#ifndef LWCGL_GL11_COMPAT_H
#define LWCGL_GL11_COMPAT_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
void lwcgl_glFog(GLenum pname, const LWCGLBuffer *params);
int lwcgl_glGetIntegerValue(GLenum pname);
void lwcgl_glGetIntegerBuffer(GLenum pname, IntBuffer *params);
void lwcgl_glSelectBuffer(IntBuffer *buffer);
GLuint lwcgl_glGenTexture(void);
void lwcgl_glGenTexturesBuffer(IntBuffer *textures);
void lwcgl_glDeleteTexture(GLuint texture);
void lwcgl_glVertexPointer3(GLint size, GLsizei stride, const LWCGLBuffer *pointer);
void lwcgl_glVertexPointer4(GLint size, GLenum type, GLsizei stride, const ByteBuffer *pointer);
void lwcgl_glTexCoordPointer3(GLint size, GLsizei stride, const LWCGLBuffer *pointer);
void lwcgl_glTexCoordPointer4(GLint size, GLenum type, GLsizei stride, const ByteBuffer *pointer);
void lwcgl_glColorPointer3(GLint size, GLsizei stride, const LWCGLBuffer *pointer);
void lwcgl_glColorPointer4(GLint size, int type_or_unsigned, GLsizei stride, const ByteBuffer *pointer);
void lwcgl_glLoadMatrixBuffer(const LWCGLBuffer *matrix);
void lwcgl_glMultMatrixBuffer(const LWCGLBuffer *matrix);
void lwcgl_gluPickMatrix(GLdouble x, GLdouble y, GLdouble width, GLdouble height, const IntBuffer *viewport);
GLint lwcgl_gluBuild2DMipmaps(GLenum target, GLint components, GLsizei width, GLsizei height, GLenum format, GLenum type, const ByteBuffer *data);
#ifdef __cplusplus
}
#endif
#ifndef LWCGL_IMPLEMENTATION
#define glFog(pname_, params_) lwcgl_glFog((pname_), (const LWCGLBuffer *)(params_))
#define LWCGL_GETINTEGER_SELECT(_1,_2,NAME,...) NAME
#define glGetInteger(...) LWCGL_GETINTEGER_SELECT(__VA_ARGS__, lwcgl_glGetIntegerBuffer, lwcgl_glGetIntegerValue, unused)(__VA_ARGS__)
#define glSelectBuffer(buffer_) lwcgl_glSelectBuffer((buffer_))
#define glGenTextures() lwcgl_glGenTexture()
#define glDeleteTextures(texture_) lwcgl_glDeleteTexture((GLuint)(texture_))
#define LWCGL_GLVP_SELECT(_1,_2,_3,_4,NAME,...) NAME
#define glVertexPointer(...) LWCGL_GLVP_SELECT(__VA_ARGS__, lwcgl_glVertexPointer4, lwcgl_glVertexPointer3, unused)(__VA_ARGS__)
#define glTexCoordPointer(...) LWCGL_GLVP_SELECT(__VA_ARGS__, lwcgl_glTexCoordPointer4, lwcgl_glTexCoordPointer3, unused)(__VA_ARGS__)
#define glColorPointer(...) LWCGL_GLVP_SELECT(__VA_ARGS__, lwcgl_glColorPointer4, lwcgl_glColorPointer3, unused)(__VA_ARGS__)
#define glLoadMatrix(matrix_) lwcgl_glLoadMatrixBuffer((const LWCGLBuffer *)(matrix_))
#define glMultMatrix(matrix_) lwcgl_glMultMatrixBuffer((const LWCGLBuffer *)(matrix_))
#define gluPickMatrix(x_,y_,w_,h_,viewport_) lwcgl_gluPickMatrix((x_),(y_),(w_),(h_),(viewport_))
#define gluBuild2DMipmaps(target_,components_,width_,height_,format_,type_,data_) \
    lwcgl_gluBuild2DMipmaps((target_),(components_),(width_),(height_),(format_),(type_),(data_))
#endif
#endif
