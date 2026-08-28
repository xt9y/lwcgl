#include <lwcgl/lwcgl.h>

#include <stdio.h>
#include <string.h>

int main(void) {
    LWCGLVersion version = LWCGL.getVersionInfo();
    if (LWCGL_VERSION_MAJOR != 3 || LWCGL_VERSION_MINOR != 4 || LWCGL_VERSION_PATCH != 2)
        return 1;
    if (version.major != 3 || version.minor != 4 || version.patch != 2)
        return 2;
    if (strcmp(LWCGL.getVersion(), "3.4.2") != 0)
        return 3;
    printf("lwcgl %s\n", LWCGL.getVersion());
    return 0;
}
