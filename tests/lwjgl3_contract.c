#include <lwcgl/lwcgl.h>

static void compile_lwjgl3_surface(void) {
    LWCGLMemoryStack *stack = MemoryStack.stackPush();
    char *title = MemoryStack.UTF8(stack, "lwcgl 3.4.2");

    if (!GLFW.glfwInit()) return;
    GLFW.glfwDefaultWindowHints();
    GLFW.glfwWindowHint(GLFW.GLFW_VISIBLE, GLFW.GLFW_FALSE);
    GLFW.glfwWindowHint(GLFW.GLFW_CONTEXT_VERSION_MAJOR, 3);
    GLFW.glfwWindowHint(GLFW.GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFW.glfwWindowHint(GLFW.GLFW_OPENGL_PROFILE, GLFW.GLFW_OPENGL_CORE_PROFILE);

    LWCGLFWwindow *window = GLFW.glfwCreateWindow(640, 480, title, NULL, NULL);
    if (window) {
        GLFW.glfwMakeContextCurrent(window);
        GLCapabilities *caps = GL.createCapabilities();
        if (caps) {
            GL11.glViewport(0, 0, 640, 480);
            GL11.glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
            GL11.glClear((GLbitfield)(GL11.GL_COLOR_BUFFER_BIT | GL11.GL_DEPTH_BUFFER_BIT));

            GLuint vao = 0;
            GLuint vbo = 0;
            GL30.glGenVertexArrays(1, &vao);
            GL30.glBindVertexArray(vao);
            GL15.glGenBuffers(1, &vbo);
            GL15.glBindBuffer((GLenum)GL15.GL_ARRAY_BUFFER, vbo);
            GL15.glBufferData((GLenum)GL15.GL_ARRAY_BUFFER, 0, NULL, (GLenum)GL15.GL_STATIC_DRAW);

            GLuint shader = GL20.glCreateShader((GLenum)GL20.GL_VERTEX_SHADER);
            GL20.glDeleteShader(shader);
            GL15.glDeleteBuffers(1, &vbo);
            GL30.glDeleteVertexArrays(1, &vao);
            GL.destroyCapabilities(caps);
        }
        GLFW.glfwDestroyWindow(window);
    }
    GLFW.glfwTerminate();
    MemoryStack.stackPop(stack);
}

int main(void) {
    if (0) compile_lwjgl3_surface();
    return 0;
}
