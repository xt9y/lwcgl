#define LWCGL_ENABLE_LWJGL2_COMPAT 1
#include <lwcgl/lwcgl.h>
int main() {
    Display.setDisplayMode(new DisplayMode(320, 200));
    FloatBuffer *m = BufferUtils.createFloatBuffer(16);
    glLoadMatrix(m);
    BufferUtils.destroy(m);
    return 0;
}
