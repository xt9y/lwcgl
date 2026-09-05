#include <lwcgl/lwcgl.h>
#include <cassert>
int main() {
    IntBuffer *i = BufferUtils.createIntBuffer(2);
    FloatBuffer *f = BufferUtils.createFloatBuffer(2);
    assert(i && f);
    assert(i->scalarType == LWCGL_BUFFER_INT);
    assert(f->scalarType == LWCGL_BUFFER_FLOAT);
    assert(Buffer.putInt(i, 7) == 0);
    assert(Buffer.putFloat((FloatBuffer *)i, 1.0f) == -1);
    Buffer.flip(i);
    assert(Buffer.getInt(i) == 7);
    BufferUtils.destroy(i); BufferUtils.destroy(f);
    return 0;
}
