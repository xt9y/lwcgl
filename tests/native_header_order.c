#include <GL/gl.h>
#include <GL/glu.h>
#include <lwcgl/lwcgl.h>
#ifdef glFog
#error canonical GL names must not be macro-redefined by lwcgl.h
#endif
int main(void) { return Display.abiVersion == LWCGL_ABI_VERSION ? 0 : 1; }
