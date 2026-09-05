#ifndef LWCGL_COMMON_H
#define LWCGL_COMMON_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
#define LWCGL_THREAD_LOCAL thread_local
extern "C" {
#else
#define LWCGL_THREAD_LOCAL _Thread_local
#endif
typedef int LWCGLbool;
enum { LWCGL_FALSE = 0, LWCGL_TRUE = 1, LWCGL_ABI_VERSION = 2 };
typedef enum LWCGLResultCode {
    LWCGL_OK = 0,
    LWCGL_ERROR_INVALID_ARGUMENT = 1,
    LWCGL_ERROR_OUT_OF_MEMORY = 2,
    LWCGL_ERROR_NO_CONTEXT = 3,
    LWCGL_ERROR_CONTEXT_MISMATCH = 4,
    LWCGL_ERROR_UNSUPPORTED = 5,
    LWCGL_ERROR_NATIVE = 6,
    LWCGL_ERROR_STATE = 7,
    LWCGL_ERROR_OVERFLOW = 8
} LWCGLResultCode;
#ifdef __cplusplus
}
#endif
#endif
