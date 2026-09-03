#include <lwcgl/lwcgl.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

void lwcglDisplayUpdateNoMessages(void)
{
    GLFWwindow *window = (GLFWwindow *)Display.getNativeWindow();

    if (window)
    {
        glfwSwapBuffers(window);
    }
}
