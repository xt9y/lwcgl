#include <lwcgl/lwcgl.h>
#include <assert.h>

int main(void) {
    assert(GLFW.glfwInit());
    assert(lwcglGLFWVersionSupported());
    GLFW.glfwDefaultWindowHints();
    GLFW.glfwWindowHint(LWCGL_GLFW_VISIBLE, LWCGL_GLFW_FALSE);
    LWCGLFWwindow *window = GLFW.glfwCreateWindow(64, 64, "lwcgl-runtime-test", 0, 0);
    assert(window);
    GLFW.glfwMakeContextCurrent(window);

    GLCapabilities *caps = GL.createCapabilities();
    assert(caps);
    assert(GL.getCapabilities() == caps);
    assert(GL.isFunctionAvailable("glClear") == LWCGL_TRUE);
    assert(GL11.glClear && GL11.glClearColor && GL11.glReadPixels);

    GL11.glViewport(0, 0, 64, 64);
    GL11.glClearColor(0.25f, 0.50f, 0.75f, 1.0f);
    GL11.glClear(LWCGL_GL_COLOR_BUFFER_BIT);
    unsigned char pixel[4] = {0, 0, 0, 0};
    GL11.glReadPixels(32, 32, 1, 1, LWCGL_GL_RGBA, LWCGL_GL_UNSIGNED_BYTE, pixel);
    assert(pixel[0] >= 60 && pixel[0] <= 68);
    assert(pixel[1] >= 124 && pixel[1] <= 132);
    assert(pixel[2] >= 187 && pixel[2] <= 195);
    assert(pixel[3] >= 250);

    GL.destroyCapabilities(caps);
    GLFW.glfwMakeContextCurrent(0);
    GLFW.glfwDestroyWindow(window);
    GLFW.glfwTerminate();
    return 0;
}
