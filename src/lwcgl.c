#include <lwcgl/lwcgl.h>

#include "lwcgl_internal.h"

#include <stdio.h>

static _Thread_local char g_lwcgl_error[512];

void lwcglSetErrorInternal(const char *message) {
    if (!message) message = "unknown error";
    snprintf(g_lwcgl_error, sizeof(g_lwcgl_error), "%s", message);
}

const char *lwcglGetLastError(void) {
    return g_lwcgl_error[0] ? g_lwcgl_error : NULL;
}

void lwcglClearError(void) {
    g_lwcgl_error[0] = '\0';
}

static int version_major(void) { return LWCGL_VERSION_MAJOR; }
static int version_minor(void) { return LWCGL_VERSION_MINOR; }
static int version_patch(void) { return LWCGL_VERSION_PATCH; }

const char *lwcglGetVersion(void) {
    return LWCGL_VERSION_STRING;
}

LWCGLVersion lwcglGetVersionInfo(void) {
    LWCGLVersion version = {
        LWCGL_VERSION_MAJOR,
        LWCGL_VERSION_MINOR,
        LWCGL_VERSION_PATCH,
        LWCGL_VERSION_STRING
    };
    return version;
}

LWCGLAPI LWCGL = {
    version_major,
    version_minor,
    version_patch,
    lwcglGetVersion,
    lwcglGetVersionInfo
};
