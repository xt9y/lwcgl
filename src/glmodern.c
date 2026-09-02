#define LWCGL_IMPLEMENTATION
#include <lwcgl/lwcgl.h>
#include <lwcgl/context.h>
#include <lwcgl/glmodern.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <string.h>

GL15API GL15;
GL20API GL20;
GL30API GL30;
GL31API GL31;
GL32API GL32;
GL33API GL33;
GL42API GL42;
GL43API GL43;
GLModernAPI GLModern;

static LWCGLbool g_modern_available;
static char g_missing_function[96];
static int g_gl_major;
static int g_gl_minor;

static int g_requested_major = 2;
static int g_requested_minor = 1;
static int g_requested_profile = LWCGL_CONTEXT_ANY_PROFILE;
static int g_requested_debug;

void lwcglSetContextVersion(int major, int minor) {
    if (major < 1 || minor < 0) {
        g_requested_major = 2;
        g_requested_minor = 1;
        return;
    }
    g_requested_major = major;
    g_requested_minor = minor;
}

void lwcglSetContextProfile(int profile) {
    if (profile != LWCGL_CONTEXT_CORE_PROFILE &&
        profile != LWCGL_CONTEXT_COMPATIBILITY_PROFILE)
        profile = LWCGL_CONTEXT_ANY_PROFILE;
    g_requested_profile = profile;
}

void lwcglSetContextDebug(int enabled) {
    g_requested_debug = enabled ? 1 : 0;
}

int lwcglRequestedContextMajorVersion(void) { return g_requested_major; }
int lwcglRequestedContextMinorVersion(void) { return g_requested_minor; }
int lwcglRequestedContextProfile(void) { return g_requested_profile; }
int lwcglRequestedContextDebug(void) { return g_requested_debug; }

static void clear_tables(void) {
    memset(&GL15, 0, sizeof(GL15));
    memset(&GL20, 0, sizeof(GL20));
    memset(&GL30, 0, sizeof(GL30));
    memset(&GL31, 0, sizeof(GL31));
    memset(&GL32, 0, sizeof(GL32));
    memset(&GL33, 0, sizeof(GL33));
    memset(&GL42, 0, sizeof(GL42));
    memset(&GL43, 0, sizeof(GL43));
    memset(&GLModern, 0, sizeof(GLModern));
}

static void remember_missing(const char *name) {
    if (!g_missing_function[0] && name)
        snprintf(g_missing_function, sizeof(g_missing_function), "%s", name);
}

static GLFWglproc resolve_proc(const char *name) {
    GLFWglproc proc = glfwGetProcAddress(name);
    if (!proc) remember_missing(name);
    return proc;
}

#define LWCGL_LOAD(group_, member_, symbol_) do {                                      \
    GLFWglproc lwcgl_proc_ = resolve_proc(#symbol_);                                    \
    _Static_assert(sizeof((group_).member_) == sizeof(lwcgl_proc_),                     \
                   "OpenGL function pointer size mismatch");                           \
    if (lwcgl_proc_)                                                                     \
        memcpy(&(group_).member_, &lwcgl_proc_, sizeof((group_).member_));               \
} while (0)

static void read_version(void) {
    const char *version = (const char *)glGetString(GL_VERSION);
    int major = 0;
    int minor = 0;

    if (version)
        (void)sscanf(version, "%d.%d", &major, &minor);

    g_gl_major = major;
    g_gl_minor = minor;
}

int lwcglLoadModernGL(void) {
    clear_tables();
    g_modern_available = LWCGL_FALSE;
    g_missing_function[0] = '\0';
    g_gl_major = 0;
    g_gl_minor = 0;

    read_version();

    LWCGL_LOAD(GL15, glGenBuffers, glGenBuffers);
    LWCGL_LOAD(GL15, glDeleteBuffers, glDeleteBuffers);
    LWCGL_LOAD(GL15, glBindBuffer, glBindBuffer);
    LWCGL_LOAD(GL15, glBufferData, glBufferData);
    LWCGL_LOAD(GL15, glBufferSubData, glBufferSubData);
    LWCGL_LOAD(GL15, glGetBufferSubData, glGetBufferSubData);

    LWCGL_LOAD(GL20, glCreateShader, glCreateShader);
    LWCGL_LOAD(GL20, glShaderSource, glShaderSource);
    LWCGL_LOAD(GL20, glCompileShader, glCompileShader);
    LWCGL_LOAD(GL20, glGetShaderiv, glGetShaderiv);
    LWCGL_LOAD(GL20, glGetShaderInfoLog, glGetShaderInfoLog);
    LWCGL_LOAD(GL20, glDeleteShader, glDeleteShader);
    LWCGL_LOAD(GL20, glCreateProgram, glCreateProgram);
    LWCGL_LOAD(GL20, glAttachShader, glAttachShader);
    LWCGL_LOAD(GL20, glDetachShader, glDetachShader);
    LWCGL_LOAD(GL20, glLinkProgram, glLinkProgram);
    LWCGL_LOAD(GL20, glGetProgramiv, glGetProgramiv);
    LWCGL_LOAD(GL20, glGetProgramInfoLog, glGetProgramInfoLog);
    LWCGL_LOAD(GL20, glUseProgram, glUseProgram);
    LWCGL_LOAD(GL20, glDeleteProgram, glDeleteProgram);
    LWCGL_LOAD(GL20, glGetUniformLocation, glGetUniformLocation);
    LWCGL_LOAD(GL20, glUniform1i, glUniform1i);
    LWCGL_LOAD(GL20, glUniform1f, glUniform1f);
    LWCGL_LOAD(GL20, glUniform2f, glUniform2f);
    LWCGL_LOAD(GL20, glUniform3f, glUniform3f);
    LWCGL_LOAD(GL20, glUniform4f, glUniform4f);
    LWCGL_LOAD(GL20, glUniformMatrix4fv, glUniformMatrix4fv);
    LWCGL_LOAD(GL20, glBindAttribLocation, glBindAttribLocation);
    LWCGL_LOAD(GL20, glEnableVertexAttribArray, glEnableVertexAttribArray);
    LWCGL_LOAD(GL20, glDisableVertexAttribArray, glDisableVertexAttribArray);
    LWCGL_LOAD(GL20, glVertexAttribPointer, glVertexAttribPointer);
    LWCGL_LOAD(GL20, glDrawBuffers, glDrawBuffers);

    LWCGL_LOAD(GL30, glGenVertexArrays, glGenVertexArrays);
    LWCGL_LOAD(GL30, glDeleteVertexArrays, glDeleteVertexArrays);
    LWCGL_LOAD(GL30, glBindVertexArray, glBindVertexArray);
    LWCGL_LOAD(GL30, glGenFramebuffers, glGenFramebuffers);
    LWCGL_LOAD(GL30, glDeleteFramebuffers, glDeleteFramebuffers);
    LWCGL_LOAD(GL30, glBindFramebuffer, glBindFramebuffer);
    LWCGL_LOAD(GL30, glFramebufferTexture2D, glFramebufferTexture2D);
    LWCGL_LOAD(GL30, glFramebufferTextureLayer, glFramebufferTextureLayer);
    LWCGL_LOAD(GL30, glCheckFramebufferStatus, glCheckFramebufferStatus);
    LWCGL_LOAD(GL30, glBlitFramebuffer, glBlitFramebuffer);
    LWCGL_LOAD(GL30, glClearBufferfv, glClearBufferfv);
    LWCGL_LOAD(GL30, glGenerateMipmap, glGenerateMipmap);
    LWCGL_LOAD(GL30, glBindBufferBase, glBindBufferBase);
    LWCGL_LOAD(GL30, glMapBufferRange, glMapBufferRange);
    LWCGL_LOAD(GL30, glFlushMappedBufferRange, glFlushMappedBufferRange);
    LWCGL_LOAD(GL30, glUnmapBuffer, glUnmapBuffer);

    LWCGL_LOAD(GL31, glDrawArraysInstanced, glDrawArraysInstanced);
    LWCGL_LOAD(GL31, glDrawElementsInstanced, glDrawElementsInstanced);
    LWCGL_LOAD(GL31, glCopyBufferSubData, glCopyBufferSubData);

    LWCGL_LOAD(GL32, glFenceSync, glFenceSync);
    LWCGL_LOAD(GL32, glClientWaitSync, glClientWaitSync);
    LWCGL_LOAD(GL32, glWaitSync, glWaitSync);
    LWCGL_LOAD(GL32, glDeleteSync, glDeleteSync);

    LWCGL_LOAD(GL33, glVertexAttribDivisor, glVertexAttribDivisor);
    LWCGL_LOAD(GL33, glGenQueries, glGenQueries);
    LWCGL_LOAD(GL33, glDeleteQueries, glDeleteQueries);
    LWCGL_LOAD(GL33, glQueryCounter, glQueryCounter);
    LWCGL_LOAD(GL33, glGetQueryObjectiv, glGetQueryObjectiv);
    LWCGL_LOAD(GL33, glGetQueryObjectui64v, glGetQueryObjectui64v);

    LWCGL_LOAD(GL42, glTexStorage2D, glTexStorage2D);
    LWCGL_LOAD(GL42, glTexStorage3D, glTexStorage3D);
    LWCGL_LOAD(GL42, glBindImageTexture, glBindImageTexture);
    LWCGL_LOAD(GL42, glMemoryBarrier, glMemoryBarrier);

    LWCGL_LOAD(GL43, glDispatchCompute, glDispatchCompute);
    LWCGL_LOAD(GL43, glDispatchComputeIndirect, glDispatchComputeIndirect);

    LWCGL_LOAD(GLModern, glActiveTexture, glActiveTexture);
    LWCGL_LOAD(GLModern, glTexImage3D, glTexImage3D);
    LWCGL_LOAD(GLModern, glTexSubImage3D, glTexSubImage3D);

    if (g_missing_function[0])
        return -1;

    if (g_gl_major < 4 || (g_gl_major == 4 && g_gl_minor < 3)) {
        remember_missing("OpenGL 4.3 context");
        return -1;
    }

    g_modern_available = LWCGL_TRUE;
    return 0;
}

LWCGLbool lwcglModernGLAvailable(void) {
    return g_modern_available;
}

const char *lwcglModernGLMissingFunction(void) {
    return g_missing_function[0] ? g_missing_function : NULL;
}

int lwcglModernGLMajorVersion(void) {
    return g_gl_major;
}

int lwcglModernGLMinorVersion(void) {
    return g_gl_minor;
}

/*
 * src/lwcgl.c is compiled with glfwCreateWindow redirected to this symbol.
 * Its existing 2.1 hints are overwritten here only when the caller requests
 * a different context, preserving the old behavior by default.
 */
GLFWwindow *lwcgl_glfwCreateWindow(int width, int height, const char *title,
                                   GLFWmonitor *monitor, GLFWwindow *share) {
    GLFWwindow *window;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, g_requested_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, g_requested_minor);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, g_requested_debug ? GLFW_TRUE : GLFW_FALSE);

    if (g_requested_major > 3 || (g_requested_major == 3 && g_requested_minor >= 2)) {
        if (g_requested_profile == LWCGL_CONTEXT_CORE_PROFILE)
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        else if (g_requested_profile == LWCGL_CONTEXT_COMPATIBILITY_PROFILE)
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
        else
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
    }

    window = glfwCreateWindow(width, height, title, monitor, share);
    if (!window)
        return NULL;

    glfwMakeContextCurrent(window);
    (void)lwcglLoadModernGL();
    return window;
}
