#define LWCGL_IMPLEMENTATION
#include <lwcgl/lwcgl.h>
#include "internal.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <time.h>

static _Thread_local char g_error[512];
static LWCGLbool g_glfw_initialized;

void lwcglSetErrorInternal(const char *message) {
    if (!message) message = "unknown error";
    snprintf(g_error, sizeof g_error, "%s", message);
}
const char *lwcglGetLastError(void) { return g_error[0] ? g_error : NULL; }
void lwcglClearError(void) { g_error[0] = '\0'; }

uint64_t lwcglNanoTime(void) {
#if defined(CLOCK_MONOTONIC)
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
        return (uint64_t)ts.tv_sec * 1000000000ull + (uint64_t)ts.tv_nsec;
#endif
    return (uint64_t)(glfwGetTime() * 1000000000.0);
}

static void glfw_error(int code, const char *message) {
    char text[512];
    snprintf(text, sizeof text, "GLFW %d: %s", code, message ? message : "unknown error");
    lwcglSetErrorInternal(text);
}

int lwcglEnsureGlfw(void) {
    if (g_glfw_initialized) return 0;
    glfwSetErrorCallback(glfw_error);
    if (!glfwInit()) { lwcglSetErrorInternal("glfwInit failed"); return -1; }
    g_glfw_initialized = LWCGL_TRUE;
    return 0;
}
