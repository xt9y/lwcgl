#include <lwcgl/lwcgl.h>
#include <lwcgl/context.h>
#include <lwcgl/glmodern.h>

static void compile_modern_gl_surface() {
    lwcglSetContextVersion(4, 3);
    lwcglSetContextProfile(LWCGL_CONTEXT_COMPATIBILITY_PROFILE);
    lwcglSetContextDebug(LWCGL_FALSE);
    Display.create();

    GLuint buffer = 0;
    GL15.glGenBuffers(1, &buffer);
    GL15.glBindBuffer(GL_ARRAY_BUFFER, buffer);
    GL15.glBufferData(GL_ARRAY_BUFFER, 256, nullptr, GL_DYNAMIC_DRAW);

    GLuint vao = 0;
    GL30.glGenVertexArrays(1, &vao);
    GL30.glBindVertexArray(vao);

    GLuint vertex = GL20.glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment = GL20.glCreateShader(GL_FRAGMENT_SHADER);
    GLuint compute = GL20.glCreateShader(GL_COMPUTE_SHADER);
    GLuint program = GL20.glCreateProgram();

    GL20.glAttachShader(program, vertex);
    GL20.glAttachShader(program, fragment);
    GL20.glLinkProgram(program);
    GL20.glUseProgram(program);

    GLuint framebuffer = 0;
    GL30.glGenFramebuffers(1, &framebuffer);
    GL30.glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    (void)GL30.glCheckFramebufferStatus(GL_FRAMEBUFFER);

    GL30.glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, buffer);
    GL42.glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);
    GL43.glDispatchCompute(1, 1, 1);
    GL42.glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

    GLuint query = 0;
    GL33.glGenQueries(1, &query);
    GL33.glQueryCounter(query, GL_TIMESTAMP);
    LWCGLGLuint64 timestamp = 0;
    GL33.glGetQueryObjectui64v(query, GL_QUERY_RESULT, &timestamp);

    GL33.glDeleteQueries(1, &query);
    GL30.glDeleteFramebuffers(1, &framebuffer);
    GL20.glDeleteProgram(program);
    GL20.glDeleteShader(compute);
    GL20.glDeleteShader(fragment);
    GL20.glDeleteShader(vertex);
    GL30.glDeleteVertexArrays(1, &vao);
    GL15.glDeleteBuffers(1, &buffer);

    (void)lwcglModernGLAvailable();
    Display.destroy();
}

int main() {
    /* Compile contract only. Do not create a window in CI. */
    if (false) compile_modern_gl_surface();
    return 0;
}
