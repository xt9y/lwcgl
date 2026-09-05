#ifndef LWCGL_LWCGL_H
#define LWCGL_LWCGL_H
#include <lwcgl/common.h>
#ifdef __cplusplus
extern "C" {
#endif
enum { LWCGL_VERSION_MAJOR=3, LWCGL_VERSION_MINOR=4, LWCGL_VERSION_PATCH=2 };
#define LWCGL_VERSION_STRING "3.4.2"
#define LWCGL_VERSION_NAME "v3.4.2"
#define LWCGL_UPSTREAM_LWJGL_VERSION "3.4.2"
typedef struct LWCGLVersion { int major, minor, patch; const char *string; } LWCGLVersion;
typedef struct LWCGLAPI { size_t abiSize; uint32_t abiVersion; int (*getVersionMajor)(void); int (*getVersionMinor)(void); int (*getVersionPatch)(void); const char *(*getVersion)(void); LWCGLVersion (*getVersionInfo)(void); } LWCGLAPI;
extern const LWCGLAPI LWCGL;
const char *lwcglGetVersion(void);
LWCGLVersion lwcglGetVersionInfo(void);
LWCGLResultCode lwcglGetLastErrorCode(void);
const char *lwcglGetLastError(void);
void lwcglClearError(void);
#ifdef __cplusplus
}
#endif
#include <lwcgl/constants.h>
#include <lwcgl/memory.h>
#include <lwcgl/glfw.h>
#include <lwcgl/opengl.h>
#endif
