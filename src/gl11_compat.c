#define LWCGL_IMPLEMENTATION
#include <lwcgl/lwcgl.h>

#include <stdint.h>

static const void *buffer_pointer(const LWCGLBuffer *buffer) {
    if (!buffer || !buffer->data || buffer->position > buffer->limit)
        return NULL;
    return (const unsigned char *)buffer->data + buffer->position * buffer->elementSize;
}

static GLenum buffer_gl_type(const LWCGLBuffer *buffer) {
    if (!buffer) return 0;
    if (buffer->elementSize == sizeof(float)) return GL_FLOAT;
    if (buffer->elementSize == sizeof(double)) return GL_DOUBLE;
    if (buffer->elementSize == sizeof(int32_t)) return GL_INT;
    if (buffer->elementSize == sizeof(int16_t)) return GL_SHORT;
    if (buffer->elementSize == sizeof(int8_t)) return GL_BYTE;
    return 0;
}

void lwcgl_glFog(GLenum pname, const LWCGLBuffer *params) {
    if (!params) return;
    if (params->elementSize == sizeof(float)) {
        glFogfv(pname, (const GLfloat *)buffer_pointer(params));
    } else if (params->elementSize == sizeof(int32_t)) {
        glFogiv(pname, (const GLint *)buffer_pointer(params));
    }
}

int lwcgl_glGetIntegerValue(GLenum pname) {
    GLint value = 0;
    glGetIntegerv(pname, &value);
    return value;
}

void lwcgl_glGetIntegerBuffer(GLenum pname, IntBuffer *params) {
    if (!params) return;
    glGetIntegerv(pname, (GLint *)Buffer.address(params));
}

void lwcgl_glSelectBuffer(IntBuffer *buffer) {
    if (!buffer) return;
    glSelectBuffer((GLsizei)Buffer.remaining(buffer), (GLuint *)Buffer.address(buffer));
}

GLuint lwcgl_glGenTexture(void) {
    GLuint texture = 0;
    glGenTextures(1, &texture);
    return texture;
}

void lwcgl_glGenTexturesBuffer(IntBuffer *textures) {
    if (!textures) return;
    glGenTextures((GLsizei)Buffer.remaining(textures), (GLuint *)Buffer.address(textures));
}

void lwcgl_glDeleteTexture(GLuint texture) {
    glDeleteTextures(1, &texture);
}

void lwcgl_glVertexPointer3(GLint size, GLsizei stride, const LWCGLBuffer *pointer) {
    GLenum type = buffer_gl_type(pointer);
    if (!type) return;
    glVertexPointer(size, type, stride, buffer_pointer(pointer));
}

void lwcgl_glTexCoordPointer3(GLint size, GLsizei stride, const LWCGLBuffer *pointer) {
    GLenum type = buffer_gl_type(pointer);
    if (!type) return;
    glTexCoordPointer(size, type, stride, buffer_pointer(pointer));
}

void lwcgl_glColorPointer3(GLint size, GLsizei stride, const LWCGLBuffer *pointer) {
    GLenum type = buffer_gl_type(pointer);
    if (!type) return;
    glColorPointer(size, type, stride, buffer_pointer(pointer));
}

void lwcgl_glLoadMatrixBuffer(const LWCGLBuffer *matrix) {
    if (!matrix) return;
    if (matrix->elementSize == sizeof(float))
        glLoadMatrixf((const GLfloat *)buffer_pointer(matrix));
    else if (matrix->elementSize == sizeof(double))
        glLoadMatrixd((const GLdouble *)buffer_pointer(matrix));
}

void lwcgl_glMultMatrixBuffer(const LWCGLBuffer *matrix) {
    if (!matrix) return;
    if (matrix->elementSize == sizeof(float))
        glMultMatrixf((const GLfloat *)buffer_pointer(matrix));
    else if (matrix->elementSize == sizeof(double))
        glMultMatrixd((const GLdouble *)buffer_pointer(matrix));
}

void lwcgl_gluPickMatrix(GLdouble x, GLdouble y, GLdouble width, GLdouble height,
                         const IntBuffer *viewport) {
    if (!viewport) return;
    gluPickMatrix(x, y, width, height, (GLint *)buffer_pointer(viewport));
}

GLint lwcgl_gluBuild2DMipmaps(GLenum target, GLint components, GLsizei width,
                              GLsizei height, GLenum format, GLenum type,
                              const ByteBuffer *data) {
    return gluBuild2DMipmaps(target, components, width, height, format, type,
                             buffer_pointer(data));
}
