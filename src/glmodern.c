#define LWCGL_IMPLEMENTATION
#include <lwcgl/lwcgl.h>
#include <lwcgl/context.h>
#include <lwcgl/glmodern.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>

LWCGL_THREAD_LOCAL GL15API GL15;
LWCGL_THREAD_LOCAL GL20API GL20;
LWCGL_THREAD_LOCAL GL30API GL30;
LWCGL_THREAD_LOCAL GL31API GL31;
LWCGL_THREAD_LOCAL GL32API GL32;
LWCGL_THREAD_LOCAL GL33API GL33;
LWCGL_THREAD_LOCAL GL42API GL42;
LWCGL_THREAD_LOCAL GL43API GL43;
LWCGL_THREAD_LOCAL GLModernAPI GLModern;

static LWCGL_THREAD_LOCAL LWCGLbool g_available;
static LWCGL_THREAD_LOCAL char g_missing[96];
static LWCGL_THREAD_LOCAL int g_major;
static LWCGL_THREAD_LOCAL int g_minor;
static int version_at_least(int major, int minor) {
    return g_major > major || (g_major == major && g_minor >= minor);
}

static void clear_tables(void) {
    memset(&GL15, 0, sizeof GL15); memset(&GL20, 0, sizeof GL20);
    memset(&GL30, 0, sizeof GL30); memset(&GL31, 0, sizeof GL31);
    memset(&GL32, 0, sizeof GL32); memset(&GL33, 0, sizeof GL33);
    memset(&GL42, 0, sizeof GL42); memset(&GL43, 0, sizeof GL43);
    memset(&GLModern, 0, sizeof GLModern);
}

static void remember_missing(const char *name) {
    if (!g_missing[0] && name) snprintf(g_missing, sizeof g_missing, "%s", name);
}

static GLFWglproc resolve(const char *name, int required) {
    GLFWglproc p = glfwGetProcAddress(name);
    if (!p && required) remember_missing(name);
    return p;
}

#define LOAD(group_, member_, required_) do { \
    GLFWglproc p_ = resolve(#member_, (required_)); \
    _Static_assert(sizeof((group_).member_) == sizeof(p_), "OpenGL function pointer size mismatch"); \
    if (p_) memcpy(&(group_).member_, &p_, sizeof((group_).member_)); \
} while (0)

int lwcglLoadModernGL(void) {
    clear_tables(); g_available = LWCGL_FALSE; g_missing[0] = '\0'; g_major = g_minor = 0;
    if (!glfwGetCurrentContext()) { remember_missing("current OpenGL context"); return -1; }
    const char *version = (const char *)glGetString(GL_VERSION);
    if (!version || sscanf(version, "%d.%d", &g_major, &g_minor) != 2) {
        remember_missing("GL_VERSION"); return -1;
    }
    const int need15 = version_at_least(1,5);
    const int need20 = version_at_least(2,0);
    const int need30 = version_at_least(3,0);
    const int need31 = version_at_least(3,1);
    const int need32 = version_at_least(3,2);
    const int need33 = version_at_least(3,3);
    const int need42 = version_at_least(4,2);
    const int need43 = version_at_least(4,3);

    if (need15) {
        LOAD(GL15, glGenBuffers, 1); LOAD(GL15, glDeleteBuffers, 1); LOAD(GL15, glBindBuffer, 1);
        LOAD(GL15, glBufferData, 1); LOAD(GL15, glBufferSubData, 1); LOAD(GL15, glGetBufferSubData, 1);
    }
    if (need20) {
        LOAD(GL20, glCreateShader, 1); LOAD(GL20, glShaderSource, 1); LOAD(GL20, glCompileShader, 1);
        LOAD(GL20, glGetShaderiv, 1); LOAD(GL20, glGetShaderInfoLog, 1); LOAD(GL20, glDeleteShader, 1);
        LOAD(GL20, glCreateProgram, 1); LOAD(GL20, glAttachShader, 1); LOAD(GL20, glDetachShader, 1);
        LOAD(GL20, glLinkProgram, 1); LOAD(GL20, glGetProgramiv, 1); LOAD(GL20, glGetProgramInfoLog, 1);
        LOAD(GL20, glUseProgram, 1); LOAD(GL20, glDeleteProgram, 1); LOAD(GL20, glGetUniformLocation, 1);
        LOAD(GL20, glUniform1i, 1); LOAD(GL20, glUniform1f, 1); LOAD(GL20, glUniform2f, 1);
        LOAD(GL20, glUniform3f, 1); LOAD(GL20, glUniform4f, 1); LOAD(GL20, glUniformMatrix4fv, 1);
        LOAD(GL20, glBindAttribLocation, 1); LOAD(GL20, glEnableVertexAttribArray, 1);
        LOAD(GL20, glDisableVertexAttribArray, 1); LOAD(GL20, glVertexAttribPointer, 1); LOAD(GL20, glDrawBuffers, 1);
    }
    if (need30) {
        LOAD(GL30, glGenVertexArrays, 1); LOAD(GL30, glDeleteVertexArrays, 1); LOAD(GL30, glBindVertexArray, 1);
        LOAD(GL30, glGenFramebuffers, 1); LOAD(GL30, glDeleteFramebuffers, 1); LOAD(GL30, glBindFramebuffer, 1);
        LOAD(GL30, glFramebufferTexture2D, 1); LOAD(GL30, glFramebufferTextureLayer, 1);
        LOAD(GL30, glCheckFramebufferStatus, 1); LOAD(GL30, glBlitFramebuffer, 1); LOAD(GL30, glClearBufferfv, 1);
        LOAD(GL30, glGenerateMipmap, 1); LOAD(GL30, glBindBufferBase, 1); LOAD(GL30, glBindBufferRange, 1);
        LOAD(GL30, glMapBufferRange, 1); LOAD(GL30, glFlushMappedBufferRange, 1); LOAD(GL30, glUnmapBuffer, 1);
        LOAD(GL30, glGetStringi, 1);
    }
    if (need31) {
        LOAD(GL31, glDrawArraysInstanced, 1); LOAD(GL31, glDrawElementsInstanced, 1); LOAD(GL31, glCopyBufferSubData, 1);
    }
    if (need32) {
        LOAD(GL32, glFenceSync, 1); LOAD(GL32, glClientWaitSync, 1); LOAD(GL32, glWaitSync, 1); LOAD(GL32, glDeleteSync, 1);
    }
    if (need33) {
        LOAD(GL33, glVertexAttribDivisor, 1); LOAD(GL33, glGenQueries, 1); LOAD(GL33, glDeleteQueries, 1);
        LOAD(GL33, glQueryCounter, 1); LOAD(GL33, glGetQueryObjectiv, 1); LOAD(GL33, glGetQueryObjectui64v, 1);
    }
    if (need42) {
        LOAD(GL42, glTexStorage2D, 1); LOAD(GL42, glTexStorage3D, 1); LOAD(GL42, glBindImageTexture, 1); LOAD(GL42, glMemoryBarrier, 1);
    }
    if (need43) {
        LOAD(GL43, glDispatchCompute, 1); LOAD(GL43, glDispatchComputeIndirect, 1);
        LOAD(GL43, glMultiDrawArraysIndirect, 1); LOAD(GL43, glMultiDrawElementsIndirect, 1);
    }
    LOAD(GLModern, glActiveTexture, version_at_least(1,3));
    LOAD(GLModern, glTexImage3D, version_at_least(1,2));
    LOAD(GLModern, glTexSubImage3D, version_at_least(1,2));
    LOAD(GLModern, glPixelStorei, 0);
    LOAD(GLModern, glReadPixels, 0);
    LOAD(GLModern, glShaderBinary, version_at_least(4,1));

    if (g_missing[0]) return -1;
    if (!version_at_least(lwcglRequestedContextMajorVersion(), lwcglRequestedContextMinorVersion())) {
        remember_missing("requested OpenGL context version"); return -1;
    }
    g_available = LWCGL_TRUE;
    return 0;
}

LWCGLbool lwcglModernGLAvailable(void) { return g_available; }
const char *lwcglModernGLMissingFunction(void) { return g_missing[0] ? g_missing : NULL; }
int lwcglModernGLMajorVersion(void) { return g_major; }
int lwcglModernGLMinorVersion(void) { return g_minor; }
