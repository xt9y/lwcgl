#define LWCGL_IMPLEMENTATION
#include <lwcgl/lwcgl.h>

#include <stdint.h>

static size_t buffer_remaining_elements(const LWCGLBuffer *buffer) {
    return buffer && buffer->limit >= buffer->position ? buffer->limit - buffer->position : 0;
}

static LWCGLbool buffer_has(const LWCGLBuffer *buffer, LWCGLBufferScalarType type,
                            size_t minimum) {
    return buffer && buffer->data && buffer->scalarType == type &&
           buffer->position <= buffer->limit && buffer_remaining_elements(buffer) >= minimum;
}

static const void *buffer_pointer(const LWCGLBuffer *buffer) {
    if (!buffer || !buffer->data || buffer->position >= buffer->limit)
        return NULL;
    return (const unsigned char *)buffer->data + buffer->position * buffer->elementSize;
}

static GLenum buffer_gl_type(const LWCGLBuffer *buffer) {
    if (!buffer || !buffer_pointer(buffer)) return 0;
    switch (buffer->scalarType) {
        case LWCGL_BUFFER_BYTE: return GL_BYTE;
        case LWCGL_BUFFER_SHORT: return GL_SHORT;
        case LWCGL_BUFFER_INT: return GL_INT;
        case LWCGL_BUFFER_FLOAT: return GL_FLOAT;
        case LWCGL_BUFFER_DOUBLE: return GL_DOUBLE;
        default: return 0;
    }
}

static LWCGLbool checked_add_size(size_t a, size_t b, size_t *out) {
    if (!out || a > SIZE_MAX - b) return LWCGL_FALSE;
    *out = a + b;
    return LWCGL_TRUE;
}

static LWCGLbool checked_mul_size(size_t a, size_t b, size_t *out) {
    if (!out || (a != 0 && b > SIZE_MAX / a)) return LWCGL_FALSE;
    *out = a * b;
    return LWCGL_TRUE;
}

static LWCGLbool format_components(GLenum format, size_t *components) {
    if (!components) return LWCGL_FALSE;
    switch (format) {
        case GL_COLOR_INDEX:
        case GL_STENCIL_INDEX:
        case GL_DEPTH_COMPONENT:
        case GL_RED:
        case GL_GREEN:
        case GL_BLUE:
        case GL_ALPHA:
        case GL_LUMINANCE:
            *components = 1;
            return LWCGL_TRUE;
        case GL_LUMINANCE_ALPHA:
            *components = 2;
            return LWCGL_TRUE;
        case GL_RGB:
#ifdef GL_BGR
        case GL_BGR:
#endif
            *components = 3;
            return LWCGL_TRUE;
        case GL_RGBA:
#ifdef GL_BGRA
        case GL_BGRA:
#endif
            *components = 4;
            return LWCGL_TRUE;
        default:
            return LWCGL_FALSE;
    }
}

static LWCGLbool pixel_bytes(GLenum format, GLenum type, size_t *bytes) {
    size_t components = 0;
    size_t scalar = 0;
    if (!bytes) return LWCGL_FALSE;

    switch (type) {
#ifdef GL_UNSIGNED_BYTE_3_3_2
        case GL_UNSIGNED_BYTE_3_3_2:
#endif
#ifdef GL_UNSIGNED_BYTE_2_3_3_REV
        case GL_UNSIGNED_BYTE_2_3_3_REV:
#endif
            *bytes = 1;
            return LWCGL_TRUE;
#ifdef GL_UNSIGNED_SHORT_5_6_5
        case GL_UNSIGNED_SHORT_5_6_5:
#endif
#ifdef GL_UNSIGNED_SHORT_5_6_5_REV
        case GL_UNSIGNED_SHORT_5_6_5_REV:
#endif
#ifdef GL_UNSIGNED_SHORT_4_4_4_4
        case GL_UNSIGNED_SHORT_4_4_4_4:
#endif
#ifdef GL_UNSIGNED_SHORT_4_4_4_4_REV
        case GL_UNSIGNED_SHORT_4_4_4_4_REV:
#endif
#ifdef GL_UNSIGNED_SHORT_5_5_5_1
        case GL_UNSIGNED_SHORT_5_5_5_1:
#endif
#ifdef GL_UNSIGNED_SHORT_1_5_5_5_REV
        case GL_UNSIGNED_SHORT_1_5_5_5_REV:
#endif
            *bytes = 2;
            return LWCGL_TRUE;
#ifdef GL_UNSIGNED_INT_8_8_8_8
        case GL_UNSIGNED_INT_8_8_8_8:
#endif
#ifdef GL_UNSIGNED_INT_8_8_8_8_REV
        case GL_UNSIGNED_INT_8_8_8_8_REV:
#endif
#ifdef GL_UNSIGNED_INT_10_10_10_2
        case GL_UNSIGNED_INT_10_10_10_2:
#endif
#ifdef GL_UNSIGNED_INT_2_10_10_10_REV
        case GL_UNSIGNED_INT_2_10_10_10_REV:
#endif
            *bytes = 4;
            return LWCGL_TRUE;
        default:
            break;
    }

    if (!format_components(format, &components)) return LWCGL_FALSE;
    switch (type) {
        case GL_BYTE:
        case GL_UNSIGNED_BYTE:
            scalar = 1;
            break;
        case GL_SHORT:
        case GL_UNSIGNED_SHORT:
            scalar = 2;
            break;
        case GL_INT:
        case GL_UNSIGNED_INT:
        case GL_FLOAT:
            scalar = 4;
            break;
        default:
            return LWCGL_FALSE;
    }
    return checked_mul_size(components, scalar, bytes);
}

static LWCGLbool image_required_bytes(GLsizei width, GLsizei height, GLenum format,
                                      GLenum type, size_t *required) {
    GLint alignment = 4;
    GLint row_length = 0;
    GLint skip_rows = 0;
    GLint skip_pixels = 0;
    size_t per_pixel = 0;
    size_t row_pixels;
    size_t row_bytes;
    size_t row_stride;
    size_t start_rows;
    size_t start_pixels;
    size_t start;
    size_t prior_rows;
    size_t last_row;
    size_t temporary;

    if (!required || width <= 0 || height <= 0 || !pixel_bytes(format, type, &per_pixel))
        return LWCGL_FALSE;

    glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
    glGetIntegerv(GL_UNPACK_ROW_LENGTH, &row_length);
    glGetIntegerv(GL_UNPACK_SKIP_ROWS, &skip_rows);
    glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &skip_pixels);

    if ((alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8) ||
        row_length < 0 || skip_rows < 0 || skip_pixels < 0)
        return LWCGL_FALSE;

    row_pixels = row_length > 0 ? (size_t)row_length : (size_t)width;
    if (!checked_mul_size(row_pixels, per_pixel, &row_bytes) ||
        !checked_add_size(row_bytes, (size_t)alignment - 1u, &temporary))
        return LWCGL_FALSE;
    row_stride = (temporary / (size_t)alignment) * (size_t)alignment;

    if (!checked_mul_size((size_t)skip_rows, row_stride, &start_rows) ||
        !checked_mul_size((size_t)skip_pixels, per_pixel, &start_pixels) ||
        !checked_add_size(start_rows, start_pixels, &start) ||
        !checked_mul_size((size_t)(height - 1), row_stride, &prior_rows) ||
        !checked_mul_size((size_t)width, per_pixel, &last_row) ||
        !checked_add_size(start, prior_rows, &temporary) ||
        !checked_add_size(temporary, last_row, required))
        return LWCGL_FALSE;

    return LWCGL_TRUE;
}

void lwcgl_glFog(GLenum pname, const LWCGLBuffer *params) {
    if (buffer_has(params, LWCGL_BUFFER_FLOAT, 4)) {
        glFogfv(pname, (const GLfloat *)buffer_pointer(params));
    } else if (buffer_has(params, LWCGL_BUFFER_INT, 4)) {
        glFogiv(pname, (const GLint *)buffer_pointer(params));
    }
}

int lwcgl_glGetIntegerValue(GLenum pname) {
    GLint values[16] = {0};
    glGetIntegerv(pname, values);
    return values[0];
}

void lwcgl_glGetIntegerBuffer(GLenum pname, IntBuffer *params) {
    if (!buffer_has(params, LWCGL_BUFFER_INT, 16)) return;
    glGetIntegerv(pname, (GLint *)Buffer.address(params));
}

void lwcgl_glSelectBuffer(IntBuffer *buffer) {
    if (!buffer_has(buffer, LWCGL_BUFFER_INT, 1)) return;
    glSelectBuffer((GLsizei)Buffer.remaining(buffer), (GLuint *)Buffer.address(buffer));
}

GLuint lwcgl_glGenTexture(void) {
    GLuint texture = 0;
    glGenTextures(1, &texture);
    return texture;
}

void lwcgl_glGenTexturesBuffer(IntBuffer *textures) {
    if (!buffer_has(textures, LWCGL_BUFFER_INT, 1)) return;
    glGenTextures((GLsizei)Buffer.remaining(textures), (GLuint *)Buffer.address(textures));
}

void lwcgl_glDeleteTexture(GLuint texture) {
    glDeleteTextures(1, &texture);
}

void lwcgl_glVertexPointer3(GLint size, GLsizei stride, const LWCGLBuffer *pointer) {
    GLenum type = buffer_gl_type(pointer);
    if (!type || pointer->scalarType == LWCGL_BUFFER_BYTE || pointer->scalarType == LWCGL_BUFFER_LONG)
        return;
    glVertexPointer(size, type, stride, buffer_pointer(pointer));
}

void lwcgl_glVertexPointer4(GLint size, GLenum type, GLsizei stride, const ByteBuffer *pointer) {
    if (!buffer_has(pointer, LWCGL_BUFFER_BYTE, 1)) return;
    glVertexPointer(size, type, stride, buffer_pointer(pointer));
}

void lwcgl_glTexCoordPointer3(GLint size, GLsizei stride, const LWCGLBuffer *pointer) {
    GLenum type = buffer_gl_type(pointer);
    if (!type || pointer->scalarType == LWCGL_BUFFER_BYTE || pointer->scalarType == LWCGL_BUFFER_LONG)
        return;
    glTexCoordPointer(size, type, stride, buffer_pointer(pointer));
}

void lwcgl_glTexCoordPointer4(GLint size, GLenum type, GLsizei stride, const ByteBuffer *pointer) {
    if (!buffer_has(pointer, LWCGL_BUFFER_BYTE, 1)) return;
    glTexCoordPointer(size, type, stride, buffer_pointer(pointer));
}

void lwcgl_glColorPointer3(GLint size, GLsizei stride, const LWCGLBuffer *pointer) {
    GLenum type = buffer_gl_type(pointer);
    if (!type || (pointer->scalarType != LWCGL_BUFFER_FLOAT &&
                  pointer->scalarType != LWCGL_BUFFER_DOUBLE))
        return;
    glColorPointer(size, type, stride, buffer_pointer(pointer));
}

void lwcgl_glColorPointer4(GLint size, int type_or_unsigned, GLsizei stride,
                           const ByteBuffer *pointer) {
    GLenum type;
    if (!buffer_has(pointer, LWCGL_BUFFER_BYTE, 1)) return;
    if (type_or_unsigned == LWCGL_FALSE)
        type = GL_BYTE;
    else if (type_or_unsigned == LWCGL_TRUE)
        type = GL_UNSIGNED_BYTE;
    else
        type = (GLenum)type_or_unsigned;
    glColorPointer(size, type, stride, buffer_pointer(pointer));
}

void lwcgl_glLoadMatrixBuffer(const LWCGLBuffer *matrix) {
    if (buffer_has(matrix, LWCGL_BUFFER_FLOAT, 16))
        glLoadMatrixf((const GLfloat *)buffer_pointer(matrix));
    else if (buffer_has(matrix, LWCGL_BUFFER_DOUBLE, 16))
        glLoadMatrixd((const GLdouble *)buffer_pointer(matrix));
}

void lwcgl_glMultMatrixBuffer(const LWCGLBuffer *matrix) {
    if (buffer_has(matrix, LWCGL_BUFFER_FLOAT, 16))
        glMultMatrixf((const GLfloat *)buffer_pointer(matrix));
    else if (buffer_has(matrix, LWCGL_BUFFER_DOUBLE, 16))
        glMultMatrixd((const GLdouble *)buffer_pointer(matrix));
}

void lwcgl_gluPickMatrix(GLdouble x, GLdouble y, GLdouble width, GLdouble height,
                         const IntBuffer *viewport) {
    if (!buffer_has(viewport, LWCGL_BUFFER_INT, 4)) return;
    gluPickMatrix(x, y, width, height, (const GLint *)buffer_pointer(viewport));
}

GLint lwcgl_gluBuild2DMipmaps(GLenum target, GLint components, GLsizei width,
                              GLsizei height, GLenum format, GLenum type,
                              const ByteBuffer *data) {
    size_t required = 0;
    if (width <= 0 || height <= 0) return GLU_INVALID_VALUE;
    if (!data || data->scalarType != LWCGL_BUFFER_BYTE ||
        !image_required_bytes(width, height, format, type, &required))
        return GLU_INVALID_ENUM;
    if (required > buffer_remaining_elements(data)) return GLU_INVALID_VALUE;
    return gluBuild2DMipmaps(target, components, width, height, format, type,
                             buffer_pointer(data));
}
