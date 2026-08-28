#ifndef LWCGL_OPENGL_H
#define LWCGL_OPENGL_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef void GLvoid;
typedef signed char GLbyte;
typedef short GLshort;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned int GLuint;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;
typedef int64_t GLint64;
typedef uint64_t GLuint64;
typedef char GLchar;
typedef struct LWCGLsyncHandle *GLsync;

typedef void (*GLDEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity,
                            GLsizei length, const GLchar *message, const void *user_param);

typedef struct GLCapabilities {
    int major;
    int minor;
    LWCGLbool OpenGL11;
    LWCGLbool OpenGL12;
    LWCGLbool OpenGL13;
    LWCGLbool OpenGL14;
    LWCGLbool OpenGL15;
    LWCGLbool OpenGL20;
    LWCGLbool OpenGL21;
    LWCGLbool OpenGL30;
    LWCGLbool OpenGL31;
    LWCGLbool OpenGL32;
    LWCGLbool OpenGL33;
    LWCGLbool OpenGL40;
    LWCGLbool OpenGL41;
    LWCGLbool OpenGL42;
    LWCGLbool OpenGL43;
    LWCGLbool OpenGL44;
    LWCGLbool OpenGL45;
    LWCGLbool OpenGL46;
} GLCapabilities;

typedef struct GLAPI {
    GLCapabilities *(*createCapabilities)(void);
    void (*destroyCapabilities)(GLCapabilities *capabilities);
    GLCapabilities *(*getCapabilities)(void);
    void (*setCapabilities)(GLCapabilities *capabilities);
    void *(*getFunctionAddress)(const char *name);
    LWCGLbool (*isFunctionAvailable)(const char *name);
} GLAPI;

typedef struct GL11API {
    int GL_FALSE;
    int GL_TRUE;
    int GL_POINTS;
    int GL_LINES;
    int GL_LINE_STRIP;
    int GL_LINE_LOOP;
    int GL_TRIANGLES;
    int GL_TRIANGLE_STRIP;
    int GL_TRIANGLE_FAN;
    int GL_ZERO;
    int GL_ONE;
    int GL_SRC_ALPHA;
    int GL_ONE_MINUS_SRC_ALPHA;
    int GL_FRONT;
    int GL_BACK;
    int GL_FRONT_AND_BACK;
    int GL_CULL_FACE;
    int GL_DEPTH_TEST;
    int GL_BLEND;
    int GL_SCISSOR_TEST;
    int GL_TEXTURE_2D;
    int GL_BYTE;
    int GL_UNSIGNED_BYTE;
    int GL_SHORT;
    int GL_UNSIGNED_SHORT;
    int GL_INT;
    int GL_UNSIGNED_INT;
    int GL_FLOAT;
    int GL_DOUBLE;
    int GL_RGB;
    int GL_RGBA;
    int GL_RED;
    int GL_GREEN;
    int GL_BLUE;
    int GL_NEVER;
    int GL_LESS;
    int GL_EQUAL;
    int GL_LEQUAL;
    int GL_GREATER;
    int GL_NOTEQUAL;
    int GL_GEQUAL;
    int GL_ALWAYS;
    int GL_COLOR_BUFFER_BIT;
    int GL_DEPTH_BUFFER_BIT;
    int GL_STENCIL_BUFFER_BIT;
    int GL_VENDOR;
    int GL_RENDERER;
    int GL_VERSION;
    int GL_EXTENSIONS;
    int GL_NEAREST;
    int GL_LINEAR;
    int GL_NEAREST_MIPMAP_NEAREST;
    int GL_LINEAR_MIPMAP_NEAREST;
    int GL_NEAREST_MIPMAP_LINEAR;
    int GL_LINEAR_MIPMAP_LINEAR;
    int GL_TEXTURE_MAG_FILTER;
    int GL_TEXTURE_MIN_FILTER;
    int GL_TEXTURE_WRAP_S;
    int GL_TEXTURE_WRAP_T;
    int GL_REPEAT;
    int GL_CLAMP;

    void (*glClear)(GLbitfield mask);
    void (*glClearColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    void (*glClearDepth)(GLdouble depth);
    void (*glViewport)(GLint x, GLint y, GLsizei width, GLsizei height);
    void (*glScissor)(GLint x, GLint y, GLsizei width, GLsizei height);
    void (*glEnable)(GLenum cap);
    void (*glDisable)(GLenum cap);
    GLboolean (*glIsEnabled)(GLenum cap);
    void (*glBlendFunc)(GLenum sfactor, GLenum dfactor);
    void (*glDepthFunc)(GLenum func);
    void (*glDepthMask)(GLboolean flag);
    void (*glColorMask)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
    void (*glCullFace)(GLenum mode);
    void (*glFrontFace)(GLenum mode);
    void (*glPolygonMode)(GLenum face, GLenum mode);
    void (*glLineWidth)(GLfloat width);
    void (*glPointSize)(GLfloat size);
    void (*glDrawArrays)(GLenum mode, GLint first, GLsizei count);
    void (*glDrawElements)(GLenum mode, GLsizei count, GLenum type, const void *indices);
    void (*glGenTextures)(GLsizei n, GLuint *textures);
    void (*glDeleteTextures)(GLsizei n, const GLuint *textures);
    void (*glBindTexture)(GLenum target, GLuint texture);
    void (*glTexParameteri)(GLenum target, GLenum pname, GLint param);
    void (*glTexParameterf)(GLenum target, GLenum pname, GLfloat param);
    void (*glTexImage2D)(GLenum target, GLint level, GLint internal_format,
                         GLsizei width, GLsizei height, GLint border,
                         GLenum format, GLenum type, const void *pixels);
    void (*glTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset,
                            GLsizei width, GLsizei height, GLenum format,
                            GLenum type, const void *pixels);
    const GLubyte *(*glGetString)(GLenum name);
    void (*glGetBooleanv)(GLenum pname, GLboolean *data);
    void (*glGetIntegerv)(GLenum pname, GLint *data);
    void (*glGetFloatv)(GLenum pname, GLfloat *data);
    GLenum (*glGetError)(void);
    void (*glReadPixels)(GLint x, GLint y, GLsizei width, GLsizei height,
                         GLenum format, GLenum type, void *pixels);
} GL11API;

typedef struct GL12API {
    int GL_TEXTURE_3D;
    int GL_TEXTURE_WRAP_R;
    int GL_CLAMP_TO_EDGE;
    void (*glDrawRangeElements)(GLenum mode, GLuint start, GLuint end,
                                GLsizei count, GLenum type, const void *indices);
    void (*glTexImage3D)(GLenum target, GLint level, GLint internal_format,
                         GLsizei width, GLsizei height, GLsizei depth, GLint border,
                         GLenum format, GLenum type, const void *pixels);
    void (*glTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset,
                            GLint zoffset, GLsizei width, GLsizei height, GLsizei depth,
                            GLenum format, GLenum type, const void *pixels);
} GL12API;

typedef struct GL13API {
    int GL_TEXTURE0;
    int GL_TEXTURE1;
    int GL_ACTIVE_TEXTURE;
    int GL_MULTISAMPLE;
    int GL_SAMPLE_ALPHA_TO_COVERAGE;
    int GL_SAMPLE_COVERAGE;
    void (*glActiveTexture)(GLenum texture);
    void (*glSampleCoverage)(GLfloat value, GLboolean invert);
    void (*glCompressedTexImage2D)(GLenum target, GLint level, GLenum internal_format,
                                   GLsizei width, GLsizei height, GLint border,
                                   GLsizei image_size, const void *data);
} GL13API;

typedef struct GL14API {
    int GL_FUNC_ADD;
    int GL_FUNC_SUBTRACT;
    int GL_FUNC_REVERSE_SUBTRACT;
    int GL_MIN;
    int GL_MAX;
    void (*glBlendEquation)(GLenum mode);
    void (*glBlendFuncSeparate)(GLenum src_rgb, GLenum dst_rgb,
                                GLenum src_alpha, GLenum dst_alpha);
    void (*glPointParameterf)(GLenum pname, GLfloat param);
    void (*glPointParameterfv)(GLenum pname, const GLfloat *params);
} GL14API;

typedef struct GL15API {
    int GL_ARRAY_BUFFER;
    int GL_ELEMENT_ARRAY_BUFFER;
    int GL_STREAM_DRAW;
    int GL_STATIC_DRAW;
    int GL_DYNAMIC_DRAW;
    int GL_READ_ONLY;
    int GL_WRITE_ONLY;
    int GL_READ_WRITE;
    int GL_BUFFER_SIZE;
    int GL_BUFFER_USAGE;
    void (*glGenBuffers)(GLsizei n, GLuint *buffers);
    void (*glDeleteBuffers)(GLsizei n, const GLuint *buffers);
    void (*glBindBuffer)(GLenum target, GLuint buffer);
    void (*glBufferData)(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
    void (*glBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
    void *(*glMapBuffer)(GLenum target, GLenum access);
    GLboolean (*glUnmapBuffer)(GLenum target);
    void (*glGenQueries)(GLsizei n, GLuint *ids);
    void (*glDeleteQueries)(GLsizei n, const GLuint *ids);
    void (*glBeginQuery)(GLenum target, GLuint id);
    void (*glEndQuery)(GLenum target);
    void (*glGetQueryObjectuiv)(GLuint id, GLenum pname, GLuint *params);
} GL15API;

typedef struct GL20API {
    int GL_VERTEX_SHADER;
    int GL_FRAGMENT_SHADER;
    int GL_COMPILE_STATUS;
    int GL_LINK_STATUS;
    int GL_VALIDATE_STATUS;
    int GL_INFO_LOG_LENGTH;
    int GL_CURRENT_PROGRAM;
    GLuint (*glCreateShader)(GLenum type);
    void (*glShaderSource)(GLuint shader, GLsizei count, const GLchar *const *string, const GLint *length);
    void (*glCompileShader)(GLuint shader);
    void (*glGetShaderiv)(GLuint shader, GLenum pname, GLint *params);
    void (*glGetShaderInfoLog)(GLuint shader, GLsizei max_length, GLsizei *length, GLchar *info_log);
    void (*glDeleteShader)(GLuint shader);
    GLuint (*glCreateProgram)(void);
    void (*glAttachShader)(GLuint program, GLuint shader);
    void (*glDetachShader)(GLuint program, GLuint shader);
    void (*glLinkProgram)(GLuint program);
    void (*glValidateProgram)(GLuint program);
    void (*glGetProgramiv)(GLuint program, GLenum pname, GLint *params);
    void (*glGetProgramInfoLog)(GLuint program, GLsizei max_length, GLsizei *length, GLchar *info_log);
    void (*glUseProgram)(GLuint program);
    void (*glDeleteProgram)(GLuint program);
    GLint (*glGetUniformLocation)(GLuint program, const GLchar *name);
    void (*glUniform1i)(GLint location, GLint v0);
    void (*glUniform1f)(GLint location, GLfloat v0);
    void (*glUniform2f)(GLint location, GLfloat v0, GLfloat v1);
    void (*glUniform3f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
    void (*glUniform4f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
    void (*glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    GLint (*glGetAttribLocation)(GLuint program, const GLchar *name);
    void (*glBindAttribLocation)(GLuint program, GLuint index, const GLchar *name);
    void (*glEnableVertexAttribArray)(GLuint index);
    void (*glDisableVertexAttribArray)(GLuint index);
    void (*glVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized,
                                  GLsizei stride, const void *pointer);
} GL20API;

typedef struct GL21API {
    int GL_PIXEL_PACK_BUFFER;
    int GL_PIXEL_UNPACK_BUFFER;
    int GL_SRGB;
    int GL_SRGB8;
    int GL_SRGB8_ALPHA8;
    void (*glUniformMatrix2x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    void (*glUniformMatrix3x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    void (*glUniformMatrix2x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    void (*glUniformMatrix4x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    void (*glUniformMatrix3x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    void (*glUniformMatrix4x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
} GL21API;

typedef struct GL30API {
    int GL_MAJOR_VERSION;
    int GL_MINOR_VERSION;
    int GL_VERTEX_ARRAY_BINDING;
    int GL_FRAMEBUFFER;
    int GL_READ_FRAMEBUFFER;
    int GL_DRAW_FRAMEBUFFER;
    int GL_RENDERBUFFER;
    int GL_COLOR_ATTACHMENT0;
    int GL_DEPTH_ATTACHMENT;
    int GL_STENCIL_ATTACHMENT;
    int GL_DEPTH_STENCIL_ATTACHMENT;
    int GL_FRAMEBUFFER_COMPLETE;
    int GL_MAP_READ_BIT;
    int GL_MAP_WRITE_BIT;
    void (*glGenVertexArrays)(GLsizei n, GLuint *arrays);
    void (*glDeleteVertexArrays)(GLsizei n, const GLuint *arrays);
    void (*glBindVertexArray)(GLuint array);
    GLboolean (*glIsVertexArray)(GLuint array);
    void *(*glMapBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
    void (*glFlushMappedBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length);
    void (*glGenFramebuffers)(GLsizei n, GLuint *framebuffers);
    void (*glDeleteFramebuffers)(GLsizei n, const GLuint *framebuffers);
    void (*glBindFramebuffer)(GLenum target, GLuint framebuffer);
    GLenum (*glCheckFramebufferStatus)(GLenum target);
    void (*glFramebufferTexture2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
    void (*glGenRenderbuffers)(GLsizei n, GLuint *renderbuffers);
    void (*glDeleteRenderbuffers)(GLsizei n, const GLuint *renderbuffers);
    void (*glBindRenderbuffer)(GLenum target, GLuint renderbuffer);
    void (*glRenderbufferStorage)(GLenum target, GLenum internal_format, GLsizei width, GLsizei height);
    void (*glFramebufferRenderbuffer)(GLenum target, GLenum attachment, GLenum renderbuffer_target, GLuint renderbuffer);
    void (*glGenerateMipmap)(GLenum target);
    void (*glBlitFramebuffer)(GLint src_x0, GLint src_y0, GLint src_x1, GLint src_y1,
                              GLint dst_x0, GLint dst_y0, GLint dst_x1, GLint dst_y1,
                              GLbitfield mask, GLenum filter);
    void (*glRenderbufferStorageMultisample)(GLenum target, GLsizei samples, GLenum internal_format,
                                             GLsizei width, GLsizei height);
} GL30API;

typedef struct GL31API {
    int GL_UNIFORM_BUFFER;
    int GL_COPY_READ_BUFFER;
    int GL_COPY_WRITE_BUFFER;
    int GL_INVALID_INDEX;
    void (*glDrawArraysInstanced)(GLenum mode, GLint first, GLsizei count, GLsizei instance_count);
    void (*glDrawElementsInstanced)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instance_count);
    void (*glCopyBufferSubData)(GLenum read_target, GLenum write_target, GLintptr read_offset,
                                GLintptr write_offset, GLsizeiptr size);
    GLuint (*glGetUniformBlockIndex)(GLuint program, const GLchar *uniform_block_name);
    void (*glUniformBlockBinding)(GLuint program, GLuint uniform_block_index, GLuint uniform_block_binding);
} GL31API;

typedef struct GL32API {
    int GL_SYNC_GPU_COMMANDS_COMPLETE;
    int GL_ALREADY_SIGNALED;
    int GL_TIMEOUT_EXPIRED;
    int GL_CONDITION_SATISFIED;
    int GL_WAIT_FAILED;
    GLsync (*glFenceSync)(GLenum condition, GLbitfield flags);
    GLboolean (*glIsSync)(GLsync sync);
    void (*glDeleteSync)(GLsync sync);
    GLenum (*glClientWaitSync)(GLsync sync, GLbitfield flags, GLuint64 timeout);
    void (*glWaitSync)(GLsync sync, GLbitfield flags, GLuint64 timeout);
    void (*glDrawElementsBaseVertex)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLint base_vertex);
} GL32API;

typedef struct GL33API {
    int GL_SAMPLER_BINDING;
    int GL_TIMESTAMP;
    void (*glVertexAttribDivisor)(GLuint index, GLuint divisor);
    void (*glGenSamplers)(GLsizei count, GLuint *samplers);
    void (*glDeleteSamplers)(GLsizei count, const GLuint *samplers);
    void (*glBindSampler)(GLuint unit, GLuint sampler);
    void (*glSamplerParameteri)(GLuint sampler, GLenum pname, GLint param);
    void (*glQueryCounter)(GLuint id, GLenum target);
    void (*glGetQueryObjectui64v)(GLuint id, GLenum pname, GLuint64 *params);
} GL33API;

typedef struct GL40API {
    int GL_PATCHES;
    int GL_PATCH_VERTICES;
    int GL_TESS_CONTROL_SHADER;
    int GL_TESS_EVALUATION_SHADER;
    void (*glDrawArraysIndirect)(GLenum mode, const void *indirect);
    void (*glDrawElementsIndirect)(GLenum mode, GLenum type, const void *indirect);
    void (*glPatchParameteri)(GLenum pname, GLint value);
    void (*glBlendEquationi)(GLuint buf, GLenum mode);
    void (*glBlendFunci)(GLuint buf, GLenum src, GLenum dst);
} GL40API;

typedef struct GL41API {
    int GL_PROGRAM_SEPARABLE;
    int GL_PROGRAM_PIPELINE_BINDING;
    void (*glGenProgramPipelines)(GLsizei n, GLuint *pipelines);
    void (*glDeleteProgramPipelines)(GLsizei n, const GLuint *pipelines);
    void (*glBindProgramPipeline)(GLuint pipeline);
    void (*glUseProgramStages)(GLuint pipeline, GLbitfield stages, GLuint program);
    void (*glProgramUniform1i)(GLuint program, GLint location, GLint v0);
    void (*glProgramUniform1f)(GLuint program, GLint location, GLfloat v0);
    void (*glProgramUniformMatrix4fv)(GLuint program, GLint location, GLsizei count,
                                      GLboolean transpose, const GLfloat *value);
} GL41API;

typedef struct GL42API {
    int GL_ALL_BARRIER_BITS;
    int GL_SHADER_IMAGE_ACCESS_BARRIER_BIT;
    int GL_TEXTURE_FETCH_BARRIER_BIT;
    int GL_SHADER_STORAGE_BARRIER_BIT;
    void (*glMemoryBarrier)(GLbitfield barriers);
    void (*glTexStorage1D)(GLenum target, GLsizei levels, GLenum internal_format, GLsizei width);
    void (*glTexStorage2D)(GLenum target, GLsizei levels, GLenum internal_format, GLsizei width, GLsizei height);
    void (*glTexStorage3D)(GLenum target, GLsizei levels, GLenum internal_format, GLsizei width, GLsizei height, GLsizei depth);
    void (*glBindImageTexture)(GLuint unit, GLuint texture, GLint level, GLboolean layered,
                               GLint layer, GLenum access, GLenum format);
} GL42API;

typedef struct GL43API {
    int GL_COMPUTE_SHADER;
    int GL_SHADER_STORAGE_BUFFER;
    int GL_DEBUG_OUTPUT;
    int GL_DEBUG_OUTPUT_SYNCHRONOUS;
    void (*glDispatchCompute)(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
    void (*glDispatchComputeIndirect)(GLintptr indirect);
    void (*glDebugMessageCallback)(GLDEBUGPROC callback, const void *user_param);
    void (*glDebugMessageControl)(GLenum source, GLenum type, GLenum severity,
                                  GLsizei count, const GLuint *ids, GLboolean enabled);
    void (*glObjectLabel)(GLenum identifier, GLuint name, GLsizei length, const GLchar *label);
} GL43API;

typedef struct GL44API {
    int GL_DYNAMIC_STORAGE_BIT;
    int GL_MAP_PERSISTENT_BIT;
    int GL_MAP_COHERENT_BIT;
    void (*glBufferStorage)(GLenum target, GLsizeiptr size, const void *data, GLbitfield flags);
    void (*glBindBuffersBase)(GLenum target, GLuint first, GLsizei count, const GLuint *buffers);
    void (*glBindTextures)(GLuint first, GLsizei count, const GLuint *textures);
    void (*glBindSamplers)(GLuint first, GLsizei count, const GLuint *samplers);
    void (*glClearTexImage)(GLuint texture, GLint level, GLenum format, GLenum type, const void *data);
} GL44API;

typedef struct GL45API {
    int GL_CLIP_ORIGIN;
    int GL_CLIP_DEPTH_MODE;
    int GL_LOWER_LEFT;
    int GL_UPPER_LEFT;
    int GL_NEGATIVE_ONE_TO_ONE;
    int GL_ZERO_TO_ONE;
    void (*glCreateBuffers)(GLsizei n, GLuint *buffers);
    void (*glNamedBufferData)(GLuint buffer, GLsizeiptr size, const void *data, GLenum usage);
    void (*glNamedBufferStorage)(GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags);
    void (*glNamedBufferSubData)(GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data);
    void (*glCreateVertexArrays)(GLsizei n, GLuint *arrays);
    void (*glCreateTextures)(GLenum target, GLsizei n, GLuint *textures);
    void (*glTextureStorage2D)(GLuint texture, GLsizei levels, GLenum internal_format, GLsizei width, GLsizei height);
    void (*glTextureSubImage2D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset,
                                GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
    void (*glBindTextureUnit)(GLuint unit, GLuint texture);
    void (*glCreateFramebuffers)(GLsizei n, GLuint *framebuffers);
    void (*glNamedFramebufferTexture)(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
    GLenum (*glCheckNamedFramebufferStatus)(GLuint framebuffer, GLenum target);
    void (*glClipControl)(GLenum origin, GLenum depth);
} GL45API;

typedef struct GL46API {
    int GL_SHADER_BINARY_FORMAT_SPIR_V;
    int GL_SPIR_V_BINARY;
    int GL_PARAMETER_BUFFER;
    void (*glSpecializeShader)(GLuint shader, const GLchar *entry_point,
                               GLuint num_specialization_constants,
                               const GLuint *constant_index, const GLuint *constant_value);
    void (*glMultiDrawArraysIndirectCount)(GLenum mode, const void *indirect,
                                           GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);
    void (*glMultiDrawElementsIndirectCount)(GLenum mode, GLenum type, const void *indirect,
                                             GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);
} GL46API;

extern GLAPI GL;
extern GL11API GL11;
extern GL12API GL12;
extern GL13API GL13;
extern GL14API GL14;
extern GL15API GL15;
extern GL20API GL20;
extern GL21API GL21;
extern GL30API GL30;
extern GL31API GL31;
extern GL32API GL32;
extern GL33API GL33;
extern GL40API GL40;
extern GL41API GL41;
extern GL42API GL42;
extern GL43API GL43;
extern GL44API GL44;
extern GL45API GL45;
extern GL46API GL46;

#ifdef __cplusplus
}
#endif

#endif
