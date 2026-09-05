#include <lwcgl/lwcgl.h>
int main(void){
    if(!GLFW.glfwInit()) return 1;
    GLFW.glfwDefaultWindowHints();
    LWCGLFWwindow *window=GLFW.glfwCreateWindow(640,480,"lwcgl",0,0);
    if(!window){GLFW.glfwTerminate();return 1;}
    GLFW.glfwMakeContextCurrent(window);
    GLCapabilities *caps=GL.createCapabilities();
    if(!caps){GLFW.glfwDestroyWindow(window);GLFW.glfwTerminate();return 1;}
    GL11.glClearColor(0.1f,0.1f,0.1f,1.0f);
    GL11.glClear(LWCGL_GL_COLOR_BUFFER_BIT|LWCGL_GL_DEPTH_BUFFER_BIT);
    GLFW.glfwSwapBuffers(window);
    GL.destroyCapabilities(caps);
    GLFW.glfwMakeContextCurrent(0);
    GLFW.glfwDestroyWindow(window);
    GLFW.glfwTerminate();
    return 0;
}
