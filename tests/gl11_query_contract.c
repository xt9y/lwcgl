#define LWCGL_IMPLEMENTATION
#include <lwcgl/lwcgl.h>

#include <assert.h>

BufferAPI Buffer = {0};

static int g_get_integer_calls;

void glGetIntegerv(GLenum pname, GLint *params) {
    assert(pname == GL_VIEWPORT);
    params[0] = 11;
    params[1] = 22;
    params[2] = 33;
    params[3] = 44;
    g_get_integer_calls++;
}

int main(void) {
    assert(lwcgl_glGetIntegerValue(GL_VIEWPORT) == 11);
    assert(g_get_integer_calls == 1);
    return 0;
}
