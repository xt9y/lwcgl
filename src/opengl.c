#include <lwcgl/lwcgl.h>

#include "lwcgl_internal.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static _Thread_local GLCapabilities *g_capabilities;

GL11API GL11 = {
    .GL_FALSE = 0,
    .GL_TRUE = 1,
    .GL_POINTS = 0x0000,
    .GL_LINES = 0x0001,
    .GL_LINE_STRIP = 0x0003,
    .GL_LINE_LOOP = 0x0002,
    .GL_TRIANGLES = 0x0004,
    .GL_TRIANGLE_STRIP = 0x0005,
    .GL_TRIANGLE_FAN = 0x0006,
    .GL_ZERO = 0,
    .GL_ONE = 1,
    .GL_SRC_ALPHA = 0x0302,
    .GL_ONE_MINUS_SRC_ALPHA = 0x0303,
    .GL_FRONT = 0x0404,
    .GL_BACK = 0x0405,
    .GL_FRONT_AND_BACK = 0x0408,
    .GL_CULL_FACE = 0x0B44,
    .GL_DEPTH_TEST = 0x0B71,
    .GL_BLEND = 0x0BE2,
    .GL_SCISSOR_TEST = 0x0C11,
    .GL_TEXTURE_2D = 0x0DE1,
    .GL_BYTE = 0x1400,
    .GL_UNSIGNED_BYTE = 0x1401,
    .GL_SHORT = 0x1402,
    .GL_UNSIGNED_SHORT = 0x1403,
    .GL_INT = 0x1404,
    .GL_UNSIGNED_INT = 0x1405,
    .GL_FLOAT = 0x1406,
    .GL_DOUBLE = 0x140A,
    .GL_RGB = 0x1907,
    .GL_RGBA = 0x1908,
    .GL_RED = 0x1903,
    .GL_GREEN = 0x1904,
    .GL_BLUE = 0x1905,
    .GL_NEVER = 0x0200,
    .GL_LESS = 0x0201,
    .GL_EQUAL = 0x0202,
    .GL_LEQUAL = 0x0203,
    .GL_GREATER = 0x0204,
    .GL_NOTEQUAL = 0x0205,
    .GL_GEQUAL = 0x0206,
    .GL_ALWAYS = 0x0207,
    .GL_COLOR_BUFFER_BIT = 0x00004000,
    .GL_DEPTH_BUFFER_BIT = 0x00000100,
    .GL_STENCIL_BUFFER_BIT = 0x00000400,
    .GL_VENDOR = 0x1F00,
    .GL_RENDERER = 0x1F01,
    .GL_VERSION = 0x1F02,
    .GL_EXTENSIONS = 0x1F03,
    .GL_NEAREST = 0x2600,
    .GL_LINEAR = 0x2601,
    .GL_NEAREST_MIPMAP_NEAREST = 0x2700,
    .GL_LINEAR_MIPMAP_NEAREST = 0x2701,
    .GL_NEAREST_MIPMAP_LINEAR = 0x2702,
    .GL_LINEAR_MIPMAP_LINEAR = 0x2703,
    .GL_TEXTURE_MAG_FILTER = 0x2800,
    .GL_TEXTURE_MIN_FILTER = 0x2801,
    .GL_TEXTURE_WRAP_S = 0x2802,
    .GL_TEXTURE_WRAP_T = 0x2803,
    .GL_REPEAT = 0x2901,
    .GL_CLAMP = 0x2900
};

GL12API GL12 = {
    .GL_TEXTURE_3D = 0x806F,
    .GL_TEXTURE_WRAP_R = 0x8072,
    .GL_CLAMP_TO_EDGE = 0x812F
};

GL13API GL13 = {
    .GL_TEXTURE0 = 0x84C0,
    .GL_TEXTURE1 = 0x84C1,
    .GL_ACTIVE_TEXTURE = 0x84E0,
    .GL_MULTISAMPLE = 0x809D,
    .GL_SAMPLE_ALPHA_TO_COVERAGE = 0x809E,
    .GL_SAMPLE_COVERAGE = 0x80A0
};

GL14API GL14 = {
    .GL_FUNC_ADD = 0x8006,
    .GL_FUNC_SUBTRACT = 0x800A,
    .GL_FUNC_REVERSE_SUBTRACT = 0x800B,
    .GL_MIN = 0x8007,
    .GL_MAX = 0x8008
};

GL15API GL15 = {
    .GL_ARRAY_BUFFER = 0x8892,
    .GL_ELEMENT_ARRAY_BUFFER = 0x8893,
    .GL_STREAM_DRAW = 0x88E0,
    .GL_STATIC_DRAW = 0x88E4,
    .GL_DYNAMIC_DRAW = 0x88E8,
    .GL_READ_ONLY = 0x88B8,
    .GL_WRITE_ONLY = 0x88B9,
    .GL_READ_WRITE = 0x88BA,
    .GL_BUFFER_SIZE = 0x8764,
    .GL_BUFFER_USAGE = 0x8765
};

GL20API GL20 = {
    .GL_VERTEX_SHADER = 0x8B31,
    .GL_FRAGMENT_SHADER = 0x8B30,
    .GL_COMPILE_STATUS = 0x8B81,
    .GL_LINK_STATUS = 0x8B82,
    .GL_VALIDATE_STATUS = 0x8B83,
    .GL_INFO_LOG_LENGTH = 0x8B84,
    .GL_CURRENT_PROGRAM = 0x8B8D
};

GL21API GL21 = {
    .GL_PIXEL_PACK_BUFFER = 0x88EB,
    .GL_PIXEL_UNPACK_BUFFER = 0x88EC,
    .GL_SRGB = 0x8C40,
    .GL_SRGB8 = 0x8C41,
    .GL_SRGB8_ALPHA8 = 0x8C43
};

GL30API GL30 = {
    .GL_MAJOR_VERSION = 0x821B,
    .GL_MINOR_VERSION = 0x821C,
    .GL_VERTEX_ARRAY_BINDING = 0x85B5,
    .GL_FRAMEBUFFER = 0x8D40,
    .GL_READ_FRAMEBUFFER = 0x8CA8,
    .GL_DRAW_FRAMEBUFFER = 0x8CA9,
    .GL_RENDERBUFFER = 0x8D41,
    .GL_COLOR_ATTACHMENT0 = 0x8CE0,
    .GL_DEPTH_ATTACHMENT = 0x8D00,
    .GL_STENCIL_ATTACHMENT = 0x8D20,
    .GL_DEPTH_STENCIL_ATTACHMENT = 0x821A,
    .GL_FRAMEBUFFER_COMPLETE = 0x8CD5,
    .GL_MAP_READ_BIT = 0x0001,
    .GL_MAP_WRITE_BIT = 0x0002
};

GL31API GL31 = {
    .GL_UNIFORM_BUFFER = 0x8A11,
    .GL_COPY_READ_BUFFER = 0x8F36,
    .GL_COPY_WRITE_BUFFER = 0x8F37,
    .GL_INVALID_INDEX = -1
};

GL32API GL32 = {
    .GL_SYNC_GPU_COMMANDS_COMPLETE = 0x9117,
    .GL_ALREADY_SIGNALED = 0x911A,
    .GL_TIMEOUT_EXPIRED = 0x911B,
    .GL_CONDITION_SATISFIED = 0x911C,
    .GL_WAIT_FAILED = 0x911D
};

GL33API GL33 = {
    .GL_SAMPLER_BINDING = 0x8919,
    .GL_TIMESTAMP = 0x8E28
};

GL40API GL40 = {
    .GL_PATCHES = 0x000E,
    .GL_PATCH_VERTICES = 0x8E72,
    .GL_TESS_CONTROL_SHADER = 0x8E88,
    .GL_TESS_EVALUATION_SHADER = 0x8E87
};

GL41API GL41 = {
    .GL_PROGRAM_SEPARABLE = 0x8258,
    .GL_PROGRAM_PIPELINE_BINDING = 0x825A
};

GL42API GL42 = {
    .GL_ALL_BARRIER_BITS = -1,
    .GL_SHADER_IMAGE_ACCESS_BARRIER_BIT = 0x00000020,
    .GL_TEXTURE_FETCH_BARRIER_BIT = 0x00000008,
    .GL_SHADER_STORAGE_BARRIER_BIT = 0x00002000
};

GL43API GL43 = {
    .GL_COMPUTE_SHADER = 0x91B9,
    .GL_SHADER_STORAGE_BUFFER = 0x90D2,
    .GL_DEBUG_OUTPUT = 0x92E0,
    .GL_DEBUG_OUTPUT_SYNCHRONOUS = 0x8242
};

GL44API GL44 = {
    .GL_DYNAMIC_STORAGE_BIT = 0x0100,
    .GL_MAP_PERSISTENT_BIT = 0x0040,
    .GL_MAP_COHERENT_BIT = 0x0080
};

GL45API GL45 = {
    .GL_CLIP_ORIGIN = 0x935C,
    .GL_CLIP_DEPTH_MODE = 0x935D,
    .GL_LOWER_LEFT = 0x8CA1,
    .GL_UPPER_LEFT = 0x8CA2,
    .GL_NEGATIVE_ONE_TO_ONE = 0x935E,
    .GL_ZERO_TO_ONE = 0x935F
};

GL46API GL46 = {
    .GL_SHADER_BINARY_FORMAT_SPIR_V = 0x9551,
    .GL_SPIR_V_BINARY = 0x9552,
    .GL_PARAMETER_BUFFER = 0x80EE
};

static void load_slot(void *slot, size_t slot_size, const char *name) {
    GLFWglproc proc = glfwGetProcAddress(name);
    const size_t copy_size = slot_size < sizeof(proc) ? slot_size : sizeof(proc);
    memset(slot, 0, slot_size);
    memcpy(slot, &proc, copy_size);
}

#define LOAD(api_, member_) load_slot(&(api_).member_, sizeof((api_).member_), #member_)

static void load_function_tables(void) {
    LOAD(GL11, glClear);
    LOAD(GL11, glClearColor);
    LOAD(GL11, glClearDepth);
    LOAD(GL11, glViewport);
    LOAD(GL11, glScissor);
    LOAD(GL11, glEnable);
    LOAD(GL11, glDisable);
    LOAD(GL11, glIsEnabled);
    LOAD(GL11, glBlendFunc);
    LOAD(GL11, glDepthFunc);
    LOAD(GL11, glDepthMask);
    LOAD(GL11, glColorMask);
    LOAD(GL11, glCullFace);
    LOAD(GL11, glFrontFace);
    LOAD(GL11, glPolygonMode);
    LOAD(GL11, glLineWidth);
    LOAD(GL11, glPointSize);
    LOAD(GL11, glDrawArrays);
    LOAD(GL11, glDrawElements);
    LOAD(GL11, glGenTextures);
    LOAD(GL11, glDeleteTextures);
    LOAD(GL11, glBindTexture);
    LOAD(GL11, glTexParameteri);
    LOAD(GL11, glTexParameterf);
    LOAD(GL11, glTexImage2D);
    LOAD(GL11, glTexSubImage2D);
    LOAD(GL11, glGetString);
    LOAD(GL11, glGetBooleanv);
    LOAD(GL11, glGetIntegerv);
    LOAD(GL11, glGetFloatv);
    LOAD(GL11, glGetError);
    LOAD(GL11, glReadPixels);

    LOAD(GL12, glDrawRangeElements);
    LOAD(GL12, glTexImage3D);
    LOAD(GL12, glTexSubImage3D);

    LOAD(GL13, glActiveTexture);
    LOAD(GL13, glSampleCoverage);
    LOAD(GL13, glCompressedTexImage2D);

    LOAD(GL14, glBlendEquation);
    LOAD(GL14, glBlendFuncSeparate);
    LOAD(GL14, glPointParameterf);
    LOAD(GL14, glPointParameterfv);

    LOAD(GL15, glGenBuffers);
    LOAD(GL15, glDeleteBuffers);
    LOAD(GL15, glBindBuffer);
    LOAD(GL15, glBufferData);
    LOAD(GL15, glBufferSubData);
    LOAD(GL15, glMapBuffer);
    LOAD(GL15, glUnmapBuffer);
    LOAD(GL15, glGenQueries);
    LOAD(GL15, glDeleteQueries);
    LOAD(GL15, glBeginQuery);
    LOAD(GL15, glEndQuery);
    LOAD(GL15, glGetQueryObjectuiv);

    LOAD(GL20, glCreateShader);
    LOAD(GL20, glShaderSource);
    LOAD(GL20, glCompileShader);
    LOAD(GL20, glGetShaderiv);
    LOAD(GL20, glGetShaderInfoLog);
    LOAD(GL20, glDeleteShader);
    LOAD(GL20, glCreateProgram);
    LOAD(GL20, glAttachShader);
    LOAD(GL20, glDetachShader);
    LOAD(GL20, glLinkProgram);
    LOAD(GL20, glValidateProgram);
    LOAD(GL20, glGetProgramiv);
    LOAD(GL20, glGetProgramInfoLog);
    LOAD(GL20, glUseProgram);
    LOAD(GL20, glDeleteProgram);
    LOAD(GL20, glGetUniformLocation);
    LOAD(GL20, glUniform1i);
    LOAD(GL20, glUniform1f);
    LOAD(GL20, glUniform2f);
    LOAD(GL20, glUniform3f);
    LOAD(GL20, glUniform4f);
    LOAD(GL20, glUniformMatrix4fv);
    LOAD(GL20, glGetAttribLocation);
    LOAD(GL20, glBindAttribLocation);
    LOAD(GL20, glEnableVertexAttribArray);
    LOAD(GL20, glDisableVertexAttribArray);
    LOAD(GL20, glVertexAttribPointer);

    LOAD(GL21, glUniformMatrix2x3fv);
    LOAD(GL21, glUniformMatrix3x2fv);
    LOAD(GL21, glUniformMatrix2x4fv);
    LOAD(GL21, glUniformMatrix4x2fv);
    LOAD(GL21, glUniformMatrix3x4fv);
    LOAD(GL21, glUniformMatrix4x3fv);

    LOAD(GL30, glGenVertexArrays);
    LOAD(GL30, glDeleteVertexArrays);
    LOAD(GL30, glBindVertexArray);
    LOAD(GL30, glIsVertexArray);
    LOAD(GL30, glMapBufferRange);
    LOAD(GL30, glFlushMappedBufferRange);
    LOAD(GL30, glGenFramebuffers);
    LOAD(GL30, glDeleteFramebuffers);
    LOAD(GL30, glBindFramebuffer);
    LOAD(GL30, glCheckFramebufferStatus);
    LOAD(GL30, glFramebufferTexture2D);
    LOAD(GL30, glGenRenderbuffers);
    LOAD(GL30, glDeleteRenderbuffers);
    LOAD(GL30, glBindRenderbuffer);
    LOAD(GL30, glRenderbufferStorage);
    LOAD(GL30, glFramebufferRenderbuffer);
    LOAD(GL30, glGenerateMipmap);
    LOAD(GL30, glBlitFramebuffer);
    LOAD(GL30, glRenderbufferStorageMultisample);

    LOAD(GL31, glDrawArraysInstanced);
    LOAD(GL31, glDrawElementsInstanced);
    LOAD(GL31, glCopyBufferSubData);
    LOAD(GL31, glGetUniformBlockIndex);
    LOAD(GL31, glUniformBlockBinding);

    LOAD(GL32, glFenceSync);
    LOAD(GL32, glIsSync);
    LOAD(GL32, glDeleteSync);
    LOAD(GL32, glClientWaitSync);
    LOAD(GL32, glWaitSync);
    LOAD(GL32, glDrawElementsBaseVertex);

    LOAD(GL33, glVertexAttribDivisor);
    LOAD(GL33, glGenSamplers);
    LOAD(GL33, glDeleteSamplers);
    LOAD(GL33, glBindSampler);
    LOAD(GL33, glSamplerParameteri);
    LOAD(GL33, glQueryCounter);
    LOAD(GL33, glGetQueryObjectui64v);

    LOAD(GL40, glDrawArraysIndirect);
    LOAD(GL40, glDrawElementsIndirect);
    LOAD(GL40, glPatchParameteri);
    LOAD(GL40, glBlendEquationi);
    LOAD(GL40, glBlendFunci);

    LOAD(GL41, glGenProgramPipelines);
    LOAD(GL41, glDeleteProgramPipelines);
    LOAD(GL41, glBindProgramPipeline);
    LOAD(GL41, glUseProgramStages);
    LOAD(GL41, glProgramUniform1i);
    LOAD(GL41, glProgramUniform1f);
    LOAD(GL41, glProgramUniformMatrix4fv);

    LOAD(GL42, glMemoryBarrier);
    LOAD(GL42, glTexStorage1D);
    LOAD(GL42, glTexStorage2D);
    LOAD(GL42, glTexStorage3D);
    LOAD(GL42, glBindImageTexture);

    LOAD(GL43, glDispatchCompute);
    LOAD(GL43, glDispatchComputeIndirect);
    LOAD(GL43, glDebugMessageCallback);
    LOAD(GL43, glDebugMessageControl);
    LOAD(GL43, glObjectLabel);

    LOAD(GL44, glBufferStorage);
    LOAD(GL44, glBindBuffersBase);
    LOAD(GL44, glBindTextures);
    LOAD(GL44, glBindSamplers);
    LOAD(GL44, glClearTexImage);

    LOAD(GL45, glCreateBuffers);
    LOAD(GL45, glNamedBufferData);
    LOAD(GL45, glNamedBufferStorage);
    LOAD(GL45, glNamedBufferSubData);
    LOAD(GL45, glCreateVertexArrays);
    LOAD(GL45, glCreateTextures);
    LOAD(GL45, glTextureStorage2D);
    LOAD(GL45, glTextureSubImage2D);
    LOAD(GL45, glBindTextureUnit);
    LOAD(GL45, glCreateFramebuffers);
    LOAD(GL45, glNamedFramebufferTexture);
    LOAD(GL45, glCheckNamedFramebufferStatus);
    LOAD(GL45, glClipControl);

    LOAD(GL46, glSpecializeShader);
    LOAD(GL46, glMultiDrawArraysIndirectCount);
    LOAD(GL46, glMultiDrawElementsIndirectCount);
}

static LWCGLbool version_at_least(int major, int minor, int required_major, int required_minor) {
    return major > required_major || (major == required_major && minor >= required_minor);
}

static void populate_capability_flags(GLCapabilities *caps) {
    const int major = caps->major;
    const int minor = caps->minor;
    caps->OpenGL11 = version_at_least(major, minor, 1, 1);
    caps->OpenGL12 = version_at_least(major, minor, 1, 2);
    caps->OpenGL13 = version_at_least(major, minor, 1, 3);
    caps->OpenGL14 = version_at_least(major, minor, 1, 4);
    caps->OpenGL15 = version_at_least(major, minor, 1, 5);
    caps->OpenGL20 = version_at_least(major, minor, 2, 0);
    caps->OpenGL21 = version_at_least(major, minor, 2, 1);
    caps->OpenGL30 = version_at_least(major, minor, 3, 0);
    caps->OpenGL31 = version_at_least(major, minor, 3, 1);
    caps->OpenGL32 = version_at_least(major, minor, 3, 2);
    caps->OpenGL33 = version_at_least(major, minor, 3, 3);
    caps->OpenGL40 = version_at_least(major, minor, 4, 0);
    caps->OpenGL41 = version_at_least(major, minor, 4, 1);
    caps->OpenGL42 = version_at_least(major, minor, 4, 2);
    caps->OpenGL43 = version_at_least(major, minor, 4, 3);
    caps->OpenGL44 = version_at_least(major, minor, 4, 4);
    caps->OpenGL45 = version_at_least(major, minor, 4, 5);
    caps->OpenGL46 = version_at_least(major, minor, 4, 6);
}

static GLCapabilities *create_capabilities(void) {
    lwcglClearError();
    if (!glfwGetCurrentContext()) {
        lwcglSetErrorInternal("GL.createCapabilities: no current GLFW OpenGL context");
        return NULL;
    }

    load_function_tables();
    if (!GL11.glGetString) {
        lwcglSetErrorInternal("GL.createCapabilities: glGetString is unavailable");
        return NULL;
    }

    const GLubyte *version_string = GL11.glGetString((GLenum)GL11.GL_VERSION);
    if (!version_string) {
        lwcglSetErrorInternal("GL.createCapabilities: OpenGL returned no version string");
        return NULL;
    }

    int major = 0;
    int minor = 0;
    if (sscanf((const char *)version_string, "%d.%d", &major, &minor) != 2) {
        lwcglSetErrorInternal("GL.createCapabilities: could not parse OpenGL version");
        return NULL;
    }

    GLCapabilities *caps = (GLCapabilities *)calloc(1, sizeof(*caps));
    if (!caps) {
        lwcglSetErrorInternal("GL.createCapabilities: allocation failed");
        return NULL;
    }

    caps->major = major;
    caps->minor = minor;
    populate_capability_flags(caps);
    g_capabilities = caps;
    return caps;
}

static void destroy_capabilities(GLCapabilities *capabilities) {
    if (!capabilities) return;
    if (g_capabilities == capabilities) g_capabilities = NULL;
    free(capabilities);
}

static GLCapabilities *get_capabilities(void) {
    return g_capabilities;
}

static void set_capabilities(GLCapabilities *capabilities) {
    g_capabilities = capabilities;
}

static void *get_function_address(const char *name) {
    if (!name) return NULL;
    GLFWglproc proc = glfwGetProcAddress(name);
    void *result = NULL;
    const size_t copy_size = sizeof(result) < sizeof(proc) ? sizeof(result) : sizeof(proc);
    memcpy(&result, &proc, copy_size);
    return result;
}

static LWCGLbool is_function_available(const char *name) {
    return name && glfwGetProcAddress(name) != NULL ? LWCGL_TRUE : LWCGL_FALSE;
}

GLAPI GL = {
    create_capabilities,
    destroy_capabilities,
    get_capabilities,
    set_capabilities,
    get_function_address,
    is_function_available
};
