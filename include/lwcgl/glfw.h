#ifndef LWCGL_GLFW_H
#define LWCGL_GLFW_H
#include <lwcgl/common.h>
#include <lwcgl/constants.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct GLFWwindow LWCGLFWwindow;
typedef struct GLFWmonitor LWCGLFWmonitor;
typedef struct GLFWcursor LWCGLFWcursor;
typedef struct LWCGLFWvidmode { int width,height,redBits,greenBits,blueBits,refreshRate; } LWCGLFWvidmode;
typedef struct LWCGLFWimage { int width,height; unsigned char *pixels; } LWCGLFWimage;
typedef struct LWCGLFWgamepadstate { unsigned char buttons[15]; float axes[6]; } LWCGLFWgamepadstate;
typedef void (*LWCGLFWglproc)(void); typedef void (*LWCGLFWerrorfun)(int,const char*); typedef void (*LWCGLFWkeyfun)(LWCGLFWwindow*,int,int,int,int); typedef void (*LWCGLFWcharfun)(LWCGLFWwindow*,unsigned int); typedef void (*LWCGLFWmousebuttonfun)(LWCGLFWwindow*,int,int,int); typedef void (*LWCGLFWcursorposfun)(LWCGLFWwindow*,double,double); typedef void (*LWCGLFWscrollfun)(LWCGLFWwindow*,double,double); typedef void (*LWCGLFWjoystickfun)(int,int);
typedef enum LWCGLFWFeature { LWCGL_GLFW_FEATURE_MONITOR_WORKAREA=1, LWCGL_GLFW_FEATURE_CONTENT_SCALE=2, LWCGL_GLFW_FEATURE_GAMEPAD=3, LWCGL_GLFW_FEATURE_CURSOR=4 } LWCGLFWFeature;
typedef struct LWCGLFWAPI {
 size_t abiSize; uint32_t abiVersion;
 int (*glfwInit)(void); void (*glfwTerminate)(void); void (*glfwGetVersion)(int*,int*,int*); const char *(*glfwGetVersionString)(void); int (*glfwGetError)(const char**); LWCGLFWerrorfun (*glfwSetErrorCallback)(LWCGLFWerrorfun);
 LWCGLFWmonitor **(*glfwGetMonitors)(int*); LWCGLFWmonitor *(*glfwGetPrimaryMonitor)(void); void (*glfwGetMonitorPos)(LWCGLFWmonitor*,int*,int*); void (*glfwGetMonitorWorkarea)(LWCGLFWmonitor*,int*,int*,int*,int*); void (*glfwGetMonitorPhysicalSize)(LWCGLFWmonitor*,int*,int*); void (*glfwGetMonitorContentScale)(LWCGLFWmonitor*,float*,float*); const char *(*glfwGetMonitorName)(LWCGLFWmonitor*); const LWCGLFWvidmode *(*glfwGetVideoModes)(LWCGLFWmonitor*,int*); const LWCGLFWvidmode *(*glfwGetVideoMode)(LWCGLFWmonitor*);
 void (*glfwDefaultWindowHints)(void); void (*glfwWindowHint)(int,int); LWCGLFWwindow *(*glfwCreateWindow)(int,int,const char*,LWCGLFWmonitor*,LWCGLFWwindow*); void (*glfwDestroyWindow)(LWCGLFWwindow*); int (*glfwWindowShouldClose)(LWCGLFWwindow*); void (*glfwSetWindowShouldClose)(LWCGLFWwindow*,int); void (*glfwSetWindowTitle)(LWCGLFWwindow*,const char*); void (*glfwSetWindowIcon)(LWCGLFWwindow*,int,const LWCGLFWimage*); void (*glfwGetWindowSize)(LWCGLFWwindow*,int*,int*); void (*glfwGetFramebufferSize)(LWCGLFWwindow*,int*,int*); LWCGLFWmonitor *(*glfwGetWindowMonitor)(LWCGLFWwindow*); void (*glfwSetWindowMonitor)(LWCGLFWwindow*,LWCGLFWmonitor*,int,int,int,int,int);
 void (*glfwPollEvents)(void); void (*glfwWaitEventsTimeout)(double);
 int (*glfwGetInputMode)(LWCGLFWwindow*,int); void (*glfwSetInputMode)(LWCGLFWwindow*,int,int); int (*glfwGetKey)(LWCGLFWwindow*,int); int (*glfwGetMouseButton)(LWCGLFWwindow*,int); void (*glfwGetCursorPos)(LWCGLFWwindow*,double*,double*); void (*glfwSetCursorPos)(LWCGLFWwindow*,double,double); LWCGLFWcursor *(*glfwCreateCursor)(const LWCGLFWimage*,int,int); LWCGLFWcursor *(*glfwCreateStandardCursor)(int); void (*glfwDestroyCursor)(LWCGLFWcursor*); void (*glfwSetCursor)(LWCGLFWwindow*,LWCGLFWcursor*); LWCGLFWkeyfun (*glfwSetKeyCallback)(LWCGLFWwindow*,LWCGLFWkeyfun); LWCGLFWcharfun (*glfwSetCharCallback)(LWCGLFWwindow*,LWCGLFWcharfun); LWCGLFWmousebuttonfun (*glfwSetMouseButtonCallback)(LWCGLFWwindow*,LWCGLFWmousebuttonfun); LWCGLFWcursorposfun (*glfwSetCursorPosCallback)(LWCGLFWwindow*,LWCGLFWcursorposfun); LWCGLFWscrollfun (*glfwSetScrollCallback)(LWCGLFWwindow*,LWCGLFWscrollfun);
 int (*glfwJoystickPresent)(int); int (*glfwJoystickIsGamepad)(int); LWCGLFWjoystickfun (*glfwSetJoystickCallback)(LWCGLFWjoystickfun); const char *(*glfwGetGamepadName)(int); int (*glfwGetGamepadState)(int,LWCGLFWgamepadstate*);
 void (*glfwSetClipboardString)(LWCGLFWwindow*,const char*); const char *(*glfwGetClipboardString)(LWCGLFWwindow*); double (*glfwGetTime)(void); uint64_t (*glfwGetTimerValue)(void); uint64_t (*glfwGetTimerFrequency)(void);
 void (*glfwMakeContextCurrent)(LWCGLFWwindow*); LWCGLFWwindow *(*glfwGetCurrentContext)(void); void (*glfwSwapBuffers)(LWCGLFWwindow*); void (*glfwSwapInterval)(int); int (*glfwExtensionSupported)(const char*); LWCGLFWglproc (*glfwGetProcAddress)(const char*);
} LWCGLFWAPI;
extern const LWCGLFWAPI GLFW;
int lwcglGLFWVersionSupported(void); int lwcglGLFWFeatureAvailable(LWCGLFWFeature feature);
#ifdef __cplusplus
}
#endif
#endif
