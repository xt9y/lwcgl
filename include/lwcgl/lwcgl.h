#ifndef LWCGL_LWCGL_H
#define LWCGL_LWCGL_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int LWCGLbool;

enum {
    LWCGL_FALSE = 0,
    LWCGL_TRUE = 1,
    LWCGL_VERSION_MAJOR = 3,
    LWCGL_VERSION_MINOR = 4,
    LWCGL_VERSION_PATCH = 2
};

#define LWCGL_VERSION_STRING "3.4.2"
#define LWCGL_VERSION_NAME "v3.4.2"
#define LWCGL_UPSTREAM_LWJGL_VERSION "3.4.2"

typedef struct LWCGLVersion {
    int major;
    int minor;
    int patch;
    const char *string;
} LWCGLVersion;

typedef struct LWCGLAPI {
    int (*getVersionMajor)(void);
    int (*getVersionMinor)(void);
    int (*getVersionPatch)(void);
    const char *(*getVersion)(void);
    LWCGLVersion (*getVersionInfo)(void);
} LWCGLAPI;

extern LWCGLAPI LWCGL;

const char *lwcglGetVersion(void);
LWCGLVersion lwcglGetVersionInfo(void);
const char *lwcglGetLastError(void);
void lwcglClearError(void);

#ifdef __cplusplus
}
#endif

#include <lwcgl/memory.h>
#include <lwcgl/glfw.h>
#include <lwcgl/opengl.h>

#endif
