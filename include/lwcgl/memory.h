#ifndef LWCGL_MEMORY_H
#define LWCGL_MEMORY_H
#include <lwcgl/common.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct LWCGLMemoryStack LWCGLMemoryStack;
typedef struct MemoryUtilAPI {
    void *(*memAlloc)(size_t); void *(*memCalloc)(size_t,size_t); void *(*memRealloc)(void*,size_t); void (*memFree)(void*);
    uintptr_t (*memAddress)(const void*); void *(*memAddressToPointer)(uintptr_t); char *(*memASCII)(const char*); char *(*memUTF8)(const char*); size_t (*memLengthNT1)(const char*);
} MemoryUtilAPI;
typedef struct MemoryStackAPI {
    LWCGLMemoryStack *(*stackPush)(void); LWCGLMemoryStack *(*stackPushCapacity)(size_t); void (*stackPop)(LWCGLMemoryStack*);
    void *(*malloc)(LWCGLMemoryStack*,size_t); void *(*calloc)(LWCGLMemoryStack*,size_t,size_t); char *(*ASCII)(LWCGLMemoryStack*,const char*); char *(*UTF8)(LWCGLMemoryStack*,const char*);
    size_t (*getPointer)(const LWCGLMemoryStack*); size_t (*getCapacity)(const LWCGLMemoryStack*); void (*trimThreadArena)(void);
} MemoryStackAPI;
extern const MemoryUtilAPI MemoryUtil;
extern const MemoryStackAPI MemoryStack;
#ifdef __cplusplus
}
#endif
#endif
