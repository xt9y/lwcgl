#include <lwcgl/lwcgl.h>

#include "lwcgl_internal.h"

#include <stdlib.h>
#include <string.h>

#define LWCGL_DEFAULT_STACK_CAPACITY (64u * 1024u)
#define LWCGL_STACK_ALIGNMENT 16u

struct LWCGLMemoryStack {
    unsigned char *memory;
    size_t capacity;
    size_t pointer;
    struct LWCGLMemoryStack *previous;
};

static _Thread_local LWCGLMemoryStack *g_stack;

static size_t align_up(size_t value, size_t alignment) {
    const size_t mask = alignment - 1u;
    return (value + mask) & ~mask;
}

static void *memory_alloc(size_t size) {
    if (size == 0) size = 1;
    void *pointer = malloc(size);
    if (!pointer) lwcglSetErrorInternal("MemoryUtil.memAlloc: allocation failed");
    return pointer;
}

static void *memory_calloc(size_t count, size_t size) {
    if (count != 0 && size > SIZE_MAX / count) {
        lwcglSetErrorInternal("MemoryUtil.memCalloc: size overflow");
        return NULL;
    }
    if (count == 0 || size == 0) {
        count = 1;
        size = 1;
    }
    void *pointer = calloc(count, size);
    if (!pointer) lwcglSetErrorInternal("MemoryUtil.memCalloc: allocation failed");
    return pointer;
}

static void *memory_realloc(void *pointer, size_t size) {
    if (size == 0) size = 1;
    void *next = realloc(pointer, size);
    if (!next) lwcglSetErrorInternal("MemoryUtil.memRealloc: allocation failed");
    return next;
}

static void memory_free(void *pointer) {
    free(pointer);
}

static uintptr_t memory_address(const void *pointer) {
    return (uintptr_t)pointer;
}

static void *memory_address_to_pointer(uintptr_t address) {
    return (void *)address;
}

static char *memory_copy_string(const char *text) {
    if (!text) return NULL;
    const size_t length = strlen(text);
    char *copy = (char *)memory_alloc(length + 1u);
    if (!copy) return NULL;
    memcpy(copy, text, length + 1u);
    return copy;
}

static size_t memory_length_nt1(const char *text) {
    return text ? strlen(text) + 1u : 0u;
}

static LWCGLMemoryStack *stack_push_capacity(size_t capacity) {
    if (capacity == 0) capacity = LWCGL_DEFAULT_STACK_CAPACITY;

    LWCGLMemoryStack *stack = (LWCGLMemoryStack *)calloc(1, sizeof(*stack));
    if (!stack) {
        lwcglSetErrorInternal("MemoryStack.stackPush: frame allocation failed");
        return NULL;
    }

    stack->memory = (unsigned char *)malloc(capacity);
    if (!stack->memory) {
        free(stack);
        lwcglSetErrorInternal("MemoryStack.stackPush: storage allocation failed");
        return NULL;
    }

    stack->capacity = capacity;
    stack->previous = g_stack;
    g_stack = stack;
    return stack;
}

static LWCGLMemoryStack *stack_push(void) {
    return stack_push_capacity(LWCGL_DEFAULT_STACK_CAPACITY);
}

static void stack_pop(LWCGLMemoryStack *stack) {
    if (!stack) return;
    if (stack != g_stack) {
        lwcglSetErrorInternal("MemoryStack.stackPop: frames must be popped in LIFO order");
        return;
    }
    g_stack = stack->previous;
    free(stack->memory);
    stack->memory = NULL;
    free(stack);
}

static void *stack_malloc(LWCGLMemoryStack *stack, size_t size) {
    if (!stack || stack != g_stack) {
        lwcglSetErrorInternal("MemoryStack.malloc: stack frame is not current");
        return NULL;
    }

    const size_t aligned = align_up(stack->pointer, LWCGL_STACK_ALIGNMENT);
    if (aligned > stack->capacity || size > stack->capacity - aligned) {
        lwcglSetErrorInternal("MemoryStack.malloc: stack overflow");
        return NULL;
    }

    void *pointer = stack->memory + aligned;
    stack->pointer = aligned + size;
    return pointer;
}

static void *stack_calloc(LWCGLMemoryStack *stack, size_t count, size_t size) {
    if (count != 0 && size > SIZE_MAX / count) {
        lwcglSetErrorInternal("MemoryStack.calloc: size overflow");
        return NULL;
    }
    const size_t bytes = count * size;
    void *pointer = stack_malloc(stack, bytes);
    if (pointer && bytes) memset(pointer, 0, bytes);
    return pointer;
}

static char *stack_string(LWCGLMemoryStack *stack, const char *text) {
    if (!text) return NULL;
    const size_t length = strlen(text) + 1u;
    char *copy = (char *)stack_malloc(stack, length);
    if (!copy) return NULL;
    memcpy(copy, text, length);
    return copy;
}

static size_t stack_get_pointer(const LWCGLMemoryStack *stack) {
    return stack ? stack->pointer : 0u;
}

static size_t stack_get_capacity(const LWCGLMemoryStack *stack) {
    return stack ? stack->capacity : 0u;
}

MemoryUtilAPI MemoryUtil = {
    memory_alloc,
    memory_calloc,
    memory_realloc,
    memory_free,
    memory_address,
    memory_address_to_pointer,
    memory_copy_string,
    memory_copy_string,
    memory_length_nt1
};

MemoryStackAPI MemoryStack = {
    stack_push,
    stack_push_capacity,
    stack_pop,
    stack_malloc,
    stack_calloc,
    stack_string,
    stack_string,
    stack_get_pointer,
    stack_get_capacity
};
