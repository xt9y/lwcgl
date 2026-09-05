#include <lwcgl/opengl.h>
#include <GLFW/glfw3.h>
#include <string.h>

struct GLFWwindow { int id; };
static struct GLFWwindow windows[2] = {{1},{2}};
static GLFWwindow *current;
static const char *missing_name;

void fake_clear_a(LWCGLbitfield mask) { (void)mask; }
void fake_clear_b(LWCGLbitfield mask) { (void)mask; }
static void fake_dummy(void) {}
static const LWCGLubyte *fake_get_string(LWCGLenum name) {
    static const LWCGLubyte version[] = "4.6 fake";
    return name == LWCGL_GL_VERSION ? version : (const LWCGLubyte *)"";
}

static GLFWglproc from_clear(void (*fn)(LWCGLbitfield)) {
    GLFWglproc out = 0;
    _Static_assert(sizeof out == sizeof fn, "function pointer size mismatch");
    memcpy(&out, &fn, sizeof out);
    return out;
}
static GLFWglproc from_string(const LWCGLubyte *(*fn)(LWCGLenum)) {
    GLFWglproc out = 0;
    _Static_assert(sizeof out == sizeof fn, "function pointer size mismatch");
    memcpy(&out, &fn, sizeof out);
    return out;
}
static GLFWglproc from_dummy(void (*fn)(void)) {
    GLFWglproc out = 0;
    _Static_assert(sizeof out == sizeof fn, "function pointer size mismatch");
    memcpy(&out, &fn, sizeof out);
    return out;
}

void fake_set_context(int id) { current = id == 1 ? &windows[0] : id == 2 ? &windows[1] : 0; }
void *fake_context_ptr(int id) { return id == 1 ? (void *)&windows[0] : id == 2 ? (void *)&windows[1] : 0; }
void fake_set_missing(const char *name) { missing_name = name; }

GLFWwindow *glfwGetCurrentContext(void) { return current; }
int glfwExtensionSupported(const char *name) { return name && strcmp(name, "GL_FAKE_extension") == 0; }
GLFWglproc glfwGetProcAddress(const char *name) {
    if (!name || (missing_name && strcmp(name, missing_name) == 0)) return 0;
    if (strcmp(name, "glGetString") == 0) return from_string(fake_get_string);
    if (strcmp(name, "glClear") == 0) return current == &windows[0] ? from_clear(fake_clear_a) : from_clear(fake_clear_b);
    return from_dummy(fake_dummy);
}
