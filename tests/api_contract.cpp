#include <lwcgl/lwcgl.h>
#include <cassert>
#include <cstring>
int main() {
    assert(BufferUtils.abiSize == sizeof(BufferUtilsAPI));
    assert(Buffer.abiVersion == LWCGL_ABI_VERSION);
    assert(Display.abiSize == sizeof(DisplayAPI));
    assert(Keyboard.getKeyIndex("F15") == Keyboard.KEY_F15);
    assert(std::strcmp(Keyboard.getKeyName(Keyboard.KEY_NUMPADENTER), "NUMPADENTER") == 0);
    assert(Mouse.abiVersion == LWCGL_ABI_VERSION);
    assert(Sys.getVersionMajor() == 2 && Sys.getVersionMinor() == 9 && Sys.getVersionPatch() == 3);
    return 0;
}
