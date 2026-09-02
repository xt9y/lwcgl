#ifndef LWCGL_GLMODERN_H
#define LWCGL_GLMODERN_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Modern OpenGL surface used by LWJGL 2.9.3-era applications.
 *
 * LWJGL 2.9.3 exposes GL15/20/30/31/32/33/42/43 classes. Native C OpenGL
 * headers are deliberately not relied on for post-1.1 prototypes because
 * their availability differs by platform. These function tables are loaded
 * from the current GLFW context with glfwGetProcAddress().
 */

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
#ifndef GL_MAP_READ_BIT
#define GL_MAP_READ_BIT 0x0001
#endif
#ifndef GL_MAP_WRITE_BIT
#define GL_MAP_WRITE_BIT 0x0002
#endif
#ifndef GL_MAP_INVALIDATE_RANGE_BIT
#define GL_MAP_INVALIDATE_RANGE_BIT 0x0004
#endif
#ifndef GL_MAP_INVALIDATE_BUFFER_BIT
#define GL_MAP_INVALIDATE_BUFFER_BIT 0x0008
#endif
#ifndef GL_MAP_FLUSH_EXPLICIT_BIT
#define GL_MAP_FLUSH_EXPLICIT_BIT 0x0010
#endif
#ifndef GL_MAP_UNSYNCHRONIZED_BIT
#define GL_MAP_UNSYNCHRONIZED_BIT 0x0020
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
#ifndef GL_COLOR_ATTACHMENT1
#define GL_COLOR_ATTACHMENT1 0x8CE1
#endif
#ifndef GL_COLOR_ATTACHMENT2
#define GL_COLOR_ATTACHMENT2 0x8CE2
#endif
#ifndef GL_COLOR_ATTACHMENT3
#define GL_COLOR_ATTACHMENT3 0x8CE3
#endif
#ifndef GL_DEPTH_ATTACHMENT
#define GL_DEPTH_ATTACHMENT 0x8D00
#endif
#ifndef GL_DEPTH_COMPONENT24
#define GL_DEPTH_COMPONENT24 0x81A6
#endif
#ifndef GL_RGBA16F
#define GL_RGBA16F 0x881A
#endif
#ifndef GL_RGBA32F
#define GL_RGBA32F 0x8814
#endif
#ifndef GL_R16F
#define GL_R16F 0x822D
#endif
#ifndef GL_R32F
#define GL_R32F 0x822E
#endif
#ifndef GL_RG16F
#define GL_RG16F 0x822F
#endif
#ifndef GL_RG32F
#define GL_RG32F 0x8230
#endif
#ifndef GL_RED
#define GL_RED 0x1903
#endif
#ifndef GL_RG
#define GL_RG 0x8227
#endif
#ifndef GL_READ_ONLY
#define GL_READ_ONLY 0x88B8
#endif
#ifndef GL_WRITE_ONLY
#define GL_WRITE_ONLY 0x88B9
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
#ifndef GL_TEXTURE1
#define GL_TEXTURE1 0x84C1
#endif
#ifndef GL_TEXTURE2
#define GL_TEXTURE2 0x84C2
#endif
#ifndef GL_TEXTURE3
#define GL_TEXTURE3 0x84C3
#endif
#ifndef GL_TEXTURE4
#define GL_TEXTURE4 0x84C4
#endif
#ifndef GL_TEXTURE5
#define GL_TEXTURE5 0x84C5
#endif
#ifndef GL_TEXTURE6
#define GL_TEXTURE6 0x84C6
#endif
#ifndef GL_TEXTURE7
#define GL_TEXTURE7 0x84C7
#endif
#ifndef GL_TEXTURE_MIN_FILTER
#define GL_TEXTURE_MIN_FILTER 0x2801
#endif
#ifndef GL_TEXTURE_MAG_FILTER
#define GL_TEXTURE_MAG_FILTER 0x2800
#endif
#ifndef GL_TEXTURE_WRAP_S
#define GL_TEXTURE_WRAP_S 0x2802
#endif
#ifndef GL_TEXTURE_WRAP_T
#define GL_TEXTURE_WRAP_T 0x2803
#endif
#ifndef GL_TEXTURE_WRAP_R
#define GL_TEXTURE_WRAP_R 0x8072
#endif
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif
#ifndef GL_NEAREST
#define GL_NEAREST 0x2600
#endif
#ifndef GL_LINEAR
#define GL_LINEAR 0x2601
#endif
#ifndef GL_SHADER_IMAGE_ACCESS_BARRIER_BIT
#define GL_SHADER_IMAGE_ACCESS_BARRIER_BIT 0x00000020
#endif
#ifndef GL_TEXTURE_FETCH_BARRIER_BIT
#define GL_TEXTURE_FETCH_BARRIER_BIT 0x00000008
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
#ifndef GL_SYNC_FLUSH_COMMANDS_BIT
#define GL_SYNC_FLUSH_COMMANDS_BIT 0x00000001
#endif
#ifndef GL_MAJOR_VERSION
#define GL_MAJOR_VERSION 0x821B
#endif
#ifndef GL_MINOR_VERSION
#define GL_MINOR_VERSION 0x821C
#endif

/* GL15: buffer objects. */
typedef struct GL15API {
    void (*glGenBuffers)(GLsizei count, GLuint *buffers);
    void (*glDeleteBuffers)(GLsizei count, const GLuint *buffers);
    void (*glBindBuffer)(GLenum target, GLuint buffer);
    void (*glBufferData)(GLenum target, LWCGLsizeiptr size, const void *data, GLenum usage);
    void (*glBufferSubData)(GLenum target, LWCGLintptr offset, LWCGLsizeiptr size, const void *data);
    void (*glGetBufferSubData)(GLenum target, LWCGLintptr offset, LWCGLsizeiptr size, void *data);
} GL15API;

/* GL20: programmable shaders, uniforms, vertex attributes and MRT. */
typedef struct GL20API {
    GLuint (*glCreateShader)(GLenum type);
    void (*glShaderSource)(GLuint shader, GLsizei count, const char *const *strings, const GLint *lengths);
    void (*glCompileShader)(GLuint shader);
    void (*glGetShaderiv)(GLuint shader, GLenum pname, GLint *value);
    void (*glGetShaderInfoLog)(GLuint shader, GLsizei buffer_size, GLsizei *length, char *log);
    void (*glDeleteShader)(GLuint shader);

    GLuint (*glCreateProgram)(void);
    void (*glAttachShader)(GLuint program, GLuint shader);
    void (*glDetachShader)(GLuint program, GLuint shader);
    void (*glLinkProgram)(GLuint program);
    void (*glGetProgramiv)(GLuint program, GLenum pname, GLint *value);
    void (*glGetProgramInfoLog)(GLuint program, GLsizei buffer_size, GLsizei *length, char *log);
    void (*glUseProgram)(GLuint program);
    void (*glDeleteProgram)(GLuint program);

    GLint (*glGetUniformLocation)(GLuint program, const char *name);
    void (*glUniform1i)(GLint location, GLint x);
    void (*glUniform1f)(GLint location, GLfloat x);
    void (*glUniform2f)(GLint location, GLfloat x, GLfloat y);
    void (*glUniform3f)(GLint location, GLfloat x, GLfloat y, GLfloat z);
    void (*glUniform4f)(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
    void (*glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

    void (*glBindAttribLocation)(GLuint program, GLuint index, const char *name);
    void (*glEnableVertexAttribArray)(GLuint index);
    void (*glDisableVertexAttribArray)(GLuint index);
    void (*glVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
    void (*glDrawBuffers)(GLsizei count, const GLenum *buffers);
} GL20API;

/* GL30: VAOs, FBOs, mapped buffers and indexed buffer binding. */
typedef struct GL30API {
    void (*glGenVertexArrays)(GLsizei count, GLuint *arrays);
    void (*glDeleteVertexArrays)(GLsizei count, const GLuint *arrays);
    void (*glBindVertexArray)(GLuint array);

    void (*glGenFramebuffers)(GLsizei count, GLuint *framebuffers);
    void (*glDeleteFramebuffers)(GLsizei count, const GLuint *framebuffers);
    void (*glBindFramebuffer)(GLenum target, GLuint framebuffer);
    void (*glFramebufferTexture2D)(GLenum target, GLenum attachment, GLenum texture_target, GLuint texture, GLint level);
    void (*glFramebufferTextureLayer)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
    GLenum (*glCheckFramebufferStatus)(GLenum target);
    void (*glBlitFramebuffer)(GLint src_x0, GLint src_y0, GLint src_x1, GLint src_y1,
                              GLint dst_x0, GLint dst_y0, GLint dst_x1, GLint dst_y1,
                              GLbitfield mask, GLenum filter);
    void (*glClearBufferfv)(GLenum buffer, GLint draw_buffer, const GLfloat *value);
    void (*glGenerateMipmap)(GLenum target);

    void (*glBindBufferBase)(GLenum target, GLuint index, GLuint buffer);
    void *(*glMapBufferRange)(GLenum target, LWCGLintptr offset, LWCGLsizeiptr length, GLbitfield access);
    void (*glFlushMappedBufferRange)(GLenum target, LWCGLintptr offset, LWCGLsizeiptr length);
    GLboolean (*glUnmapBuffer)(GLenum target);
} GL30API;

/* GL31: instanced draws and GPU-side buffer copies. */
typedef struct GL31API {
    void (*glDrawArraysInstanced)(GLenum mode, GLint first, GLsizei count, GLsizei instances);
    void (*glDrawElementsInstanced)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instances);
    void (*glCopyBufferSubData)(GLenum read_target, GLenum write_target,
                                LWCGLintptr read_offset, LWCGLintptr write_offset,
                                LWCGLsizeiptr size);
} GL31API;

/* GL32: fence synchronization for non-blocking streaming rings. */
typedef struct GL32API {
    LWCGLsync (*glFenceSync)(GLenum condition, GLbitfield flags);
    GLenum (*glClientWaitSync)(LWCGLsync sync, GLbitfield flags, LWCGLGLuint64 timeout);
    void (*glWaitSync)(LWCGLsync sync, GLbitfield flags, LWCGLGLuint64 timeout);
    void (*glDeleteSync)(LWCGLsync sync);
} GL32API;

/* GL33: vertex divisors and GPU timer queries. */
typedef struct GL33API {
    void (*glVertexAttribDivisor)(GLuint index, GLuint divisor);
    void (*glGenQueries)(GLsizei count, GLuint *queries);
    void (*glDeleteQueries)(GLsizei count, const GLuint *queries);
    void (*glQueryCounter)(GLuint query, GLenum target);
    void (*glGetQueryObjectiv)(GLuint query, GLenum pname, GLint *value);
    void (*glGetQueryObjectui64v)(GLuint query, GLenum pname, LWCGLGLuint64 *value);
} GL33API;

/* GL42: immutable texture storage, images and memory barriers. */
typedef struct GL42API {
    void (*glTexStorage2D)(GLenum target, GLsizei levels, GLenum internal_format, GLsizei width, GLsizei height);
    void (*glTexStorage3D)(GLenum target, GLsizei levels, GLenum internal_format, GLsizei width, GLsizei height, GLsizei depth);
    void (*glBindImageTexture)(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
    void (*glMemoryBarrier)(GLbitfield barriers);
} GL42API;

/* GL43: compute dispatch. */
typedef struct GL43API {
    void (*glDispatchCompute)(GLuint groups_x, GLuint groups_y, GLuint groups_z);
    void (*glDispatchComputeIndirect)(LWCGLintptr indirect);
} GL43API;

/* Common post-GL11 functions not naturally owned by the groups above. */
typedef struct GLModernAPI {
    void (*glActiveTexture)(GLenum texture);
    void (*glTexImage3D)(GLenum target, GLint level, GLint internal_format,
                         GLsizei width, GLsizei height, GLsizei depth,
                         GLint border, GLenum format, GLenum type, const void *pixels);
    void (*glTexSubImage3D)(GLenum target, GLint level,
                            GLint x, GLint y, GLint z,
                            GLsizei width, GLsizei height, GLsizei depth,
                            GLenum format, GLenum type, const void *pixels);
} GLModernAPI;

extern GL15API GL15;
extern GL20API GL20;
extern GL30API GL30;
extern GL31API GL31;
extern GL32API GL32;
extern GL33API GL33;
extern GL42API GL42;
extern GL43API GL43;
extern GLModernAPI GLModern;

/* Load the tables for the context that is current on the calling thread. */
int lwcglLoadModernGL(void);
LWCGLbool lwcglModernGLAvailable(void);
const char *lwcglModernGLMissingFunction(void);
int lwcglModernGLMajorVersion(void);
int lwcglModernGLMinorVersion(void);

#ifdef __cplusplus
}
#endif

#endif
