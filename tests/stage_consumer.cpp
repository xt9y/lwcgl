#include <lwcgl/lwcgl.h>
int main() { return Display.abiVersion == LWCGL_ABI_VERSION ? 0 : 1; }
