#define LWCGL_ENABLE_LWJGL2_COMPAT 1
#include <lwcgl/lwcgl.h>
int main() {
    LWCGLDisplayModeValue mode = {320, 200, 0, 0, LWCGL_FALSE};
    Display.setDisplayMode(&mode);
    FloatBuffer *m = BufferUtils.createFloatBuffer(16);
    if (!m) return 1;
    glLoadMatrix(m);
    BufferUtils.destroy(m);
    return 0;
}
