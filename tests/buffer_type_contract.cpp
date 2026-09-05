#include <lwcgl/lwcgl.h>

#include <cassert>

int main() {
    IntBuffer *ints = BufferUtils.createIntBuffer(4);
    FloatBuffer *floats = BufferUtils.createFloatBuffer(4);
    ByteBuffer *bytes = BufferUtils.createByteBuffer(4);

    assert(ints != nullptr && ints->scalarType == LWCGL_BUFFER_INT);
    assert(floats != nullptr && floats->scalarType == LWCGL_BUFFER_FLOAT);
    assert(bytes != nullptr && bytes->scalarType == LWCGL_BUFFER_BYTE);

    assert(Buffer.putInt(ints, 7) == 0);
    assert(Buffer.putFloat(floats, 1.0f) == 0);
    assert(Buffer.putFloat(reinterpret_cast<FloatBuffer *>(ints), 1.0f) == -1);
    assert(Buffer.putInt(reinterpret_cast<IntBuffer *>(floats), 7) == -1);

    BufferUtils.destroy(ints);
    BufferUtils.destroy(floats);
    BufferUtils.destroy(bytes);
    return 0;
}
