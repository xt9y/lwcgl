#ifndef LWCGL_MEMORY_H
#define LWCGL_MEMORY_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LWCGLMemoryStack LWCGLMemoryStack;

typedef struct MemoryUtilAPI {
    void *(*memAlloc)(size_t size);
    void *(*memCalloc)(size_t count, size_t size);
    void *(*memRealloc)(void *pointer, size_t size);
    void (*memFree)(void *pointer);
    uintptr_t (*memAddress)(const void *pointer);
    void *(*memAddressToPointer)(uintptr_t address);
    char *(*memASCII)(const char *text);
    char *(*memUTF8)(const char *text);
    size_t (*memLengthNT1)(const char *text);
} MemoryUtilAPI;

typedef struct MemoryStackAPI {
    LWCGLMemoryStack *(*stackPush)(void);
    LWCGLMemoryStack *(*stackPushCapacity)(size_t capacity);
    void (*stackPop)(LWCGLMemoryStack *stack);
    void *(*malloc)(LWCGLMemoryStack *stack, size_t size);
    void *(*calloc)(LWCGLMemoryStack *stack, size_t count, size_t size);
    char *(*ASCII)(LWCGLMemoryStack *stack, const char *text);
    char *(*UTF8)(LWCGLMemoryStack *stack, const char *text);
    size_t (*getPointer)(const LWCGLMemoryStack *stack);
    size_t (*getCapacity)(const LWCGLMemoryStack *stack);
} MemoryStackAPI;

extern MemoryUtilAPI MemoryUtil;
extern MemoryStackAPI MemoryStack;

#ifdef __cplusplus
}
#endif

#endif
