#define LWCGL_IMPLEMENTATION
#include <lwcgl/lwcgl.h>

#include <assert.h>
#include <stdint.h>
#include <string.h>

static int g_fog_float_calls;
static int g_fog_int_calls;
static int g_integer_calls;
static int g_vertex_calls;
static int g_color_calls;
static int g_matrix_calls;
static int g_pick_calls;
static int g_mipmap_calls;
static GLenum g_last_type;
static GLint g_unpack_alignment = 4;
static GLint g_unpack_row_length;
static GLint g_unpack_skip_rows;
static GLint g_unpack_skip_pixels;

static size_t test_remaining(const LWCGLBuffer *buffer) {
    return buffer && buffer->limit >= buffer->position ? buffer->limit - buffer->position : 0;
}

static void *test_address(LWCGLBuffer *buffer) {
    if (!buffer || !buffer->data || buffer->position > buffer->limit) return NULL;
    return (unsigned char *)buffer->data + buffer->position * buffer->elementSize;
}

BufferAPI Buffer = {
    .remaining = test_remaining,
    .address = test_address
};

void glFogfv(GLenum pname, const GLfloat *params) {
    (void)pname;
    (void)params;
    ++g_fog_float_calls;
}

void glFogiv(GLenum pname, const GLint *params) {
    (void)pname;
    (void)params;
    ++g_fog_int_calls;
}

void glGetIntegerv(GLenum pname, GLint *params) {
    if (pname == GL_UNPACK_ALIGNMENT) {
        *params = g_unpack_alignment;
    } else if (pname == GL_UNPACK_ROW_LENGTH) {
        *params = g_unpack_row_length;
    } else if (pname == GL_UNPACK_SKIP_ROWS) {
        *params = g_unpack_skip_rows;
    } else if (pname == GL_UNPACK_SKIP_PIXELS) {
        *params = g_unpack_skip_pixels;
    } else {
        params[0] = 1;
        params[1] = 2;
        params[2] = 3;
        params[3] = 4;
        ++g_integer_calls;
    }
}

void glSelectBuffer(GLsizei size, GLuint *buffer) {
    (void)size;
    (void)buffer;
}

void glGenTextures(GLsizei count, GLuint *textures) {
    (void)count;
    (void)textures;
}

void glDeleteTextures(GLsizei count, const GLuint *textures) {
    (void)count;
    (void)textures;
}

void glVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) {
    (void)size;
    (void)stride;
    (void)pointer;
    g_last_type = type;
    ++g_vertex_calls;
}

void glTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) {
    (void)size;
    (void)type;
    (void)stride;
    (void)pointer;
}

void glColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) {
    (void)size;
    (void)stride;
    (void)pointer;
    g_last_type = type;
    ++g_color_calls;
}

void glLoadMatrixf(const GLfloat *matrix) {
    (void)matrix;
    ++g_matrix_calls;
}

void glLoadMatrixd(const GLdouble *matrix) {
    (void)matrix;
    ++g_matrix_calls;
}

void glMultMatrixf(const GLfloat *matrix) { (void)matrix; }
void glMultMatrixd(const GLdouble *matrix) { (void)matrix; }

void gluPickMatrix(GLdouble x, GLdouble y, GLdouble width, GLdouble height,
                   const GLint *viewport) {
    (void)x;
    (void)y;
    (void)width;
    (void)height;
    (void)viewport;
    ++g_pick_calls;
}

GLint gluBuild2DMipmaps(GLenum target, GLint components, GLsizei width, GLsizei height,
                        GLenum format, GLenum type, const void *data) {
    (void)target;
    (void)components;
    (void)width;
    (void)height;
    (void)format;
    (void)type;
    (void)data;
    ++g_mipmap_calls;
    return 123;
}

int main(void) {
    int32_t integer_data[20] = {0};
    LWCGLBuffer integers = {
        integer_data, sizeof(integer_data[0]), 20, 0, 20, SIZE_MAX, LWCGL_BUFFER_INT
    };
    float matrix_data[16] = {0};
    LWCGLBuffer matrix = {
        matrix_data, sizeof(matrix_data[0]), 16, 0, 15, SIZE_MAX, LWCGL_BUFFER_FLOAT
    };
    unsigned char byte_data[64] = {0};
    LWCGLBuffer bytes = {
        byte_data, sizeof(byte_data[0]), 64, 0, 64, SIZE_MAX, LWCGL_BUFFER_BYTE
    };

    lwcgl_glVertexPointer3(3, 0, &integers);
    assert(g_vertex_calls == 1 && g_last_type == GL_INT);

    lwcgl_glFog(GL_FOG_COLOR, &integers);
    assert(g_fog_int_calls == 1 && g_fog_float_calls == 0);
    integers.limit = 3;
    lwcgl_glFog(GL_FOG_COLOR, &integers);
    assert(g_fog_int_calls == 1);

    integers.limit = 15;
    lwcgl_glGetIntegerBuffer(GL_VIEWPORT, &integers);
    assert(g_integer_calls == 0);
    integers.limit = 16;
    lwcgl_glGetIntegerBuffer(GL_VIEWPORT, &integers);
    assert(g_integer_calls == 1);

    lwcgl_glLoadMatrixBuffer(&matrix);
    assert(g_matrix_calls == 0);
    matrix.limit = 16;
    lwcgl_glLoadMatrixBuffer(&matrix);
    assert(g_matrix_calls == 1);

    integers.limit = 3;
    lwcgl_gluPickMatrix(0.0, 0.0, 1.0, 1.0, &integers);
    assert(g_pick_calls == 0);
    integers.limit = 4;
    lwcgl_gluPickMatrix(0.0, 0.0, 1.0, 1.0, &integers);
    assert(g_pick_calls == 1);

    lwcgl_glVertexPointer3(3, 0, &bytes);
    assert(g_vertex_calls == 1);
    lwcgl_glVertexPointer4(3, GL_UNSIGNED_BYTE, 0, &bytes);
    assert(g_vertex_calls == 2 && g_last_type == GL_UNSIGNED_BYTE);

    lwcgl_glColorPointer4(4, LWCGL_TRUE, 0, &bytes);
    assert(g_color_calls == 1 && g_last_type == GL_UNSIGNED_BYTE);
    lwcgl_glColorPointer4(4, LWCGL_FALSE, 0, &bytes);
    assert(g_color_calls == 2 && g_last_type == GL_BYTE);

    bytes.limit = 11;
    assert(lwcgl_gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 2, 2, GL_RGB,
                                   GL_UNSIGNED_BYTE, &bytes) == GLU_INVALID_VALUE);
    bytes.limit = 14;
    assert(lwcgl_gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 2, 2, GL_RGB,
                                   GL_UNSIGNED_BYTE, &bytes) == 123);
    assert(g_mipmap_calls == 1);

    g_unpack_row_length = 4;
    g_unpack_skip_rows = 1;
    g_unpack_skip_pixels = 1;
    bytes.limit = 32;
    assert(lwcgl_gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 2, 2, GL_RGB,
                                   GL_UNSIGNED_BYTE, &bytes) == GLU_INVALID_VALUE);
    bytes.limit = 33;
    assert(lwcgl_gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 2, 2, GL_RGB,
                                   GL_UNSIGNED_BYTE, &bytes) == 123);
    assert(g_mipmap_calls == 2);

    return 0;
}
