#define LWCGL_IMPLEMENTATION
#include <lwcgl/lwcgl.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <string.h>

GL15API GL15;
GL20API GL20;
GL30API GL30;
GL33API GL33;
GL42API GL42;
GL43API GL43;
GLModernAPI GLModern;

static LWCGLbool g_modern_available;
static char g_missing_function[96];
static int g_gl_major;
static int g_gl_minor;

static void clear_tables(void) {
    memset(&GL15, 0, sizeof(GL15));
    memset(&GL20, 0, sizeof(GL20));
    memset(&GL30, 0, sizeof(GL30));
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
    GLint major = 0;
    GLint minor = 0;

    /* GL_MAJOR_VERSION/GL_MINOR_VERSION are core from OpenGL 3.0 onward. */
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    if (major <= 0) {
        const char *version = (const char *)glGetString(GL_VERSION);
        if (version)
            (void)sscanf(version, "%d.%d", &major, &minor);
    }

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
    LWCGL_LOAD(GL30, glGenerateMipmap, glGenerateMipmap);
    LWCGL_LOAD(GL30, glBindBufferBase, glBindBufferBase);
    LWCGL_LOAD(GL30, glMapBufferRange, glMapBufferRange);
    LWCGL_LOAD(GL30, glFlushMappedBufferRange, glFlushMappedBufferRange);
    LWCGL_LOAD(GL30, glUnmapBuffer, glUnmapBuffer);

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
