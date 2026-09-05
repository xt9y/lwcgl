#include <lwcgl/lwcgl.h>
#include <assert.h>
#include <string.h>
int main(void){assert(LWCGL.abiVersion==LWCGL_ABI_VERSION);assert(LWCGL.abiSize==sizeof(LWCGLAPI));assert(strcmp(lwcglGetVersion(),"3.4.2")==0);assert(lwcglGLFWVersionSupported());return 0;}
