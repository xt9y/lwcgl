#ifndef GLFW3_H
#define GLFW3_H
#ifdef __cplusplus
extern "C" {
#endif
typedef struct GLFWwindow GLFWwindow;
typedef void (*GLFWglproc)(void);
GLFWwindow *glfwGetCurrentContext(void);
GLFWglproc glfwGetProcAddress(const char *name);
int glfwExtensionSupported(const char *name);
#ifdef __cplusplus
}
#endif
#endif
