#ifndef LWCGL_GLMODERN_H
#define LWCGL_GLMODERN_H

#include <stddef.h>
#include <stdint.h>
#include <lwcgl/lwcgl.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__cplusplus)
#define LWCGL_THREAD_LOCAL thread_local
#else
#define LWCGL_THREAD_LOCAL _Thread_local
#endif

typedef ptrdiff_t LWCGLintptr;
typedef ptrdiff_t LWCGLsizeiptr;
typedef uint64_t LWCGLGLuint64;
typedef struct LWCGLsyncOpaque *LWCGLsync;

#ifndef GL_ARRAY_BUFFER
#define GL_ARRAY_BUFFER 0x8892
#endif
#ifndef GL_ELEMENT_ARRAY_BUFFER
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#endif
#ifndef GL_DRAW_INDIRECT_BUFFER
#define GL_DRAW_INDIRECT_BUFFER 0x8F3F
#endif
#ifndef GL_COPY_READ_BUFFER
#define GL_COPY_READ_BUFFER 0x8F36
#endif
#ifndef GL_COPY_WRITE_BUFFER
#define GL_COPY_WRITE_BUFFER 0x8F37
#endif
#ifndef GL_PIXEL_PACK_BUFFER
#define GL_PIXEL_PACK_BUFFER 0x88EB
#endif
#ifndef GL_PIXEL_UNPACK_BUFFER
#define GL_PIXEL_UNPACK_BUFFER 0x88EC
#endif
#ifndef GL_UNIFORM_BUFFER
#define GL_UNIFORM_BUFFER 0x8A11
#endif
#ifndef GL_SHADER_STORAGE_BUFFER
#define GL_SHADER_STORAGE_BUFFER 0x90D2
#endif
#ifndef GL_STATIC_DRAW
#define GL_STATIC_DRAW 0x88E4
#endif
#ifndef GL_DYNAMIC_DRAW
#define GL_DYNAMIC_DRAW 0x88E8
#endif
#ifndef GL_STREAM_DRAW
#define GL_STREAM_DRAW 0x88E0
#endif
#ifndef GL_VERTEX_SHADER
#define GL_VERTEX_SHADER 0x8B31
#endif
#ifndef GL_FRAGMENT_SHADER
#define GL_FRAGMENT_SHADER 0x8B30
#endif
#ifndef GL_COMPUTE_SHADER
#define GL_COMPUTE_SHADER 0x91B9
#endif
#ifndef GL_COMPILE_STATUS
#define GL_COMPILE_STATUS 0x8B81
#endif
#ifndef GL_LINK_STATUS
#define GL_LINK_STATUS 0x8B82
#endif
#ifndef GL_INFO_LOG_LENGTH
#define GL_INFO_LOG_LENGTH 0x8B84
#endif
#ifndef GL_FRAMEBUFFER
#define GL_FRAMEBUFFER 0x8D40
#endif
#ifndef GL_READ_FRAMEBUFFER
#define GL_READ_FRAMEBUFFER 0x8CA8
#endif
#ifndef GL_DRAW_FRAMEBUFFER
#define GL_DRAW_FRAMEBUFFER 0x8CA9
#endif
#ifndef GL_FRAMEBUFFER_COMPLETE
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#endif
#ifndef GL_COLOR_ATTACHMENT0
#define GL_COLOR_ATTACHMENT0 0x8CE0
#endif
#ifndef GL_DEPTH_ATTACHMENT
#define GL_DEPTH_ATTACHMENT 0x8D00
#endif
#ifndef GL_RGBA16F
#define GL_RGBA16F 0x881A
#endif
#ifndef GL_WRITE_ONLY
#define GL_WRITE_ONLY 0x88B9
#endif
#ifndef GL_READ_ONLY
#define GL_READ_ONLY 0x88B8
#endif
#ifndef GL_READ_WRITE
#define GL_READ_WRITE 0x88BA
#endif
#ifndef GL_TEXTURE_3D
#define GL_TEXTURE_3D 0x806F
#endif
#ifndef GL_TEXTURE0
#define GL_TEXTURE0 0x84C0
#endif
#ifndef GL_SHADER_IMAGE_ACCESS_BARRIER_BIT
#define GL_SHADER_IMAGE_ACCESS_BARRIER_BIT 0x00000020
#endif
#ifndef GL_TEXTURE_FETCH_BARRIER_BIT
#define GL_TEXTURE_FETCH_BARRIER_BIT 0x00000008
#endif
#ifndef GL_COMMAND_BARRIER_BIT
#define GL_COMMAND_BARRIER_BIT 0x00000040
#endif
#ifndef GL_SHADER_STORAGE_BARRIER_BIT
#define GL_SHADER_STORAGE_BARRIER_BIT 0x00002000
#endif
#ifndef GL_BUFFER_UPDATE_BARRIER_BIT
#define GL_BUFFER_UPDATE_BARRIER_BIT 0x00000200
#endif
#ifndef GL_FRAMEBUFFER_BARRIER_BIT
#define GL_FRAMEBUFFER_BARRIER_BIT 0x00000400
#endif
#ifndef GL_ALL_BARRIER_BITS
#define GL_ALL_BARRIER_BITS 0xFFFFFFFFu
#endif
#ifndef GL_TIMESTAMP
#define GL_TIMESTAMP 0x8E28
#endif
#ifndef GL_QUERY_RESULT
#define GL_QUERY_RESULT 0x8866
#endif
#ifndef GL_QUERY_RESULT_AVAILABLE
#define GL_QUERY_RESULT_AVAILABLE 0x8867
#endif
#ifndef GL_SYNC_GPU_COMMANDS_COMPLETE
#define GL_SYNC_GPU_COMMANDS_COMPLETE 0x9117
#endif
#ifndef GL_ALREADY_SIGNALED
#define GL_ALREADY_SIGNALED 0x911A
#endif
#ifndef GL_TIMEOUT_EXPIRED
#define GL_TIMEOUT_EXPIRED 0x911B
#endif
#ifndef GL_CONDITION_SATISFIED
#define GL_CONDITION_SATISFIED 0x911C
#endif
#ifndef GL_WAIT_FAILED
#define GL_WAIT_FAILED 0x911D
#endif
#ifndef GL_MAJOR_VERSION
#define GL_MAJOR_VERSION 0x821B
#endif
#ifndef GL_MINOR_VERSION
#define GL_MINOR_VERSION 0x821C
#endif
#ifndef GL_NUM_EXTENSIONS
#define GL_NUM_EXTENSIONS 0x821D
#endif
#ifndef GL_MAP_READ_BIT
#define GL_MAP_READ_BIT 0x0001
#endif
#ifndef GL_MAP_WRITE_BIT
#define GL_MAP_WRITE_BIT 0x0002
#endif

typedef struct GL15API {
    void (*glGenBuffers)(GLsizei, GLuint *);
    void (*glDeleteBuffers)(GLsizei, const GLuint *);
    void (*glBindBuffer)(GLenum, GLuint);
    void (*glBufferData)(GLenum, LWCGLsizeiptr, const void *, GLenum);
    void (*glBufferSubData)(GLenum, LWCGLintptr, LWCGLsizeiptr, const void *);
    void (*glGetBufferSubData)(GLenum, LWCGLintptr, LWCGLsizeiptr, void *);
} GL15API;

typedef struct GL20API {
    GLuint (*glCreateShader)(GLenum);
    void (*glShaderSource)(GLuint, GLsizei, const char *const *, const GLint *);
    void (*glCompileShader)(GLuint);
    void (*glGetShaderiv)(GLuint, GLenum, GLint *);
    void (*glGetShaderInfoLog)(GLuint, GLsizei, GLsizei *, char *);
    void (*glDeleteShader)(GLuint);
    GLuint (*glCreateProgram)(void);
    void (*glAttachShader)(GLuint, GLuint);
    void (*glDetachShader)(GLuint, GLuint);
    void (*glLinkProgram)(GLuint);
    void (*glGetProgramiv)(GLuint, GLenum, GLint *);
    void (*glGetProgramInfoLog)(GLuint, GLsizei, GLsizei *, char *);
    void (*glUseProgram)(GLuint);
    void (*glDeleteProgram)(GLuint);
    GLint (*glGetUniformLocation)(GLuint, const char *);
    void (*glUniform1i)(GLint, GLint);
    void (*glUniform1f)(GLint, GLfloat);
    void (*glUniform2f)(GLint, GLfloat, GLfloat);
    void (*glUniform3f)(GLint, GLfloat, GLfloat, GLfloat);
    void (*glUniform4f)(GLint, GLfloat, GLfloat, GLfloat, GLfloat);
    void (*glUniformMatrix4fv)(GLint, GLsizei, GLboolean, const GLfloat *);
    void (*glBindAttribLocation)(GLuint, GLuint, const char *);
    void (*glEnableVertexAttribArray)(GLuint);
    void (*glDisableVertexAttribArray)(GLuint);
    void (*glVertexAttribPointer)(GLuint, GLint, GLenum, GLboolean, GLsizei, const void *);
    void (*glDrawBuffers)(GLsizei, const GLenum *);
} GL20API;

typedef struct GL30API {
    void (*glGenVertexArrays)(GLsizei, GLuint *);
    void (*glDeleteVertexArrays)(GLsizei, const GLuint *);
    void (*glBindVertexArray)(GLuint);
    void (*glGenFramebuffers)(GLsizei, GLuint *);
    void (*glDeleteFramebuffers)(GLsizei, const GLuint *);
    void (*glBindFramebuffer)(GLenum, GLuint);
    void (*glFramebufferTexture2D)(GLenum, GLenum, GLenum, GLuint, GLint);
    void (*glFramebufferTextureLayer)(GLenum, GLenum, GLuint, GLint, GLint);
    GLenum (*glCheckFramebufferStatus)(GLenum);
    void (*glBlitFramebuffer)(GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLbitfield, GLenum);
    void (*glClearBufferfv)(GLenum, GLint, const GLfloat *);
    void (*glGenerateMipmap)(GLenum);
    void (*glBindBufferBase)(GLenum, GLuint, GLuint);
    void (*glBindBufferRange)(GLenum, GLuint, GLuint, LWCGLintptr, LWCGLsizeiptr);
    void *(*glMapBufferRange)(GLenum, LWCGLintptr, LWCGLsizeiptr, GLbitfield);
    void (*glFlushMappedBufferRange)(GLenum, LWCGLintptr, LWCGLsizeiptr);
    GLboolean (*glUnmapBuffer)(GLenum);
    const GLubyte *(*glGetStringi)(GLenum, GLuint);
} GL30API;

typedef struct GL31API {
    void (*glDrawArraysInstanced)(GLenum, GLint, GLsizei, GLsizei);
    void (*glDrawElementsInstanced)(GLenum, GLsizei, GLenum, const void *, GLsizei);
    void (*glCopyBufferSubData)(GLenum, GLenum, LWCGLintptr, LWCGLintptr, LWCGLsizeiptr);
} GL31API;

typedef struct GL32API {
    LWCGLsync (*glFenceSync)(GLenum, GLbitfield);
    GLenum (*glClientWaitSync)(LWCGLsync, GLbitfield, LWCGLGLuint64);
    void (*glWaitSync)(LWCGLsync, GLbitfield, LWCGLGLuint64);
    void (*glDeleteSync)(LWCGLsync);
} GL32API;

typedef struct GL33API {
    void (*glVertexAttribDivisor)(GLuint, GLuint);
    void (*glGenQueries)(GLsizei, GLuint *);
    void (*glDeleteQueries)(GLsizei, const GLuint *);
    void (*glQueryCounter)(GLuint, GLenum);
    void (*glGetQueryObjectiv)(GLuint, GLenum, GLint *);
    void (*glGetQueryObjectui64v)(GLuint, GLenum, LWCGLGLuint64 *);
} GL33API;

typedef struct GL42API {
    void (*glTexStorage2D)(GLenum, GLsizei, GLenum, GLsizei, GLsizei);
    void (*glTexStorage3D)(GLenum, GLsizei, GLenum, GLsizei, GLsizei, GLsizei);
    void (*glBindImageTexture)(GLuint, GLuint, GLint, GLboolean, GLint, GLenum, GLenum);
    void (*glMemoryBarrier)(GLbitfield);
} GL42API;

typedef struct GL43API {
    void (*glDispatchCompute)(GLuint, GLuint, GLuint);
    void (*glDispatchComputeIndirect)(LWCGLintptr);
    void (*glMultiDrawArraysIndirect)(GLenum, const void *, GLsizei, GLsizei);
    void (*glMultiDrawElementsIndirect)(GLenum, GLenum, const void *, GLsizei, GLsizei);
} GL43API;

typedef struct GLModernAPI {
    void (*glActiveTexture)(GLenum);
    void (*glTexImage3D)(GLenum, GLint, GLint, GLsizei, GLsizei, GLsizei, GLint, GLenum, GLenum, const void *);
    void (*glTexSubImage3D)(GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLenum, const void *);
    void (*glPixelStorei)(GLenum, GLint);
    void (*glReadPixels)(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, void *);
    void (*glShaderBinary)(GLsizei, const GLuint *, GLenum, const void *, GLsizei);
} GLModernAPI;

extern LWCGL_THREAD_LOCAL GL15API GL15;
extern LWCGL_THREAD_LOCAL GL20API GL20;
extern LWCGL_THREAD_LOCAL GL30API GL30;
extern LWCGL_THREAD_LOCAL GL31API GL31;
extern LWCGL_THREAD_LOCAL GL32API GL32;
extern LWCGL_THREAD_LOCAL GL33API GL33;
extern LWCGL_THREAD_LOCAL GL42API GL42;
extern LWCGL_THREAD_LOCAL GL43API GL43;
extern LWCGL_THREAD_LOCAL GLModernAPI GLModern;

int lwcglLoadModernGL(void);
LWCGLbool lwcglModernGLAvailable(void);
const char *lwcglModernGLMissingFunction(void);
int lwcglModernGLMajorVersion(void);
int lwcglModernGLMinorVersion(void);

#ifdef __cplusplus
}
#endif
#endif
