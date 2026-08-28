#ifndef LWCGL_GLFW_H
#define LWCGL_GLFW_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct GLFWwindow LWCGLFWwindow;
typedef struct GLFWmonitor LWCGLFWmonitor;
typedef struct GLFWcursor LWCGLFWcursor;

typedef void (*LWCGLFWglproc)(void);
typedef void (*LWCGLFWerrorfun)(int error_code, const char *description);
typedef void (*LWCGLFWwindowposfun)(LWCGLFWwindow *window, int xpos, int ypos);
typedef void (*LWCGLFWwindowsizefun)(LWCGLFWwindow *window, int width, int height);
typedef void (*LWCGLFWwindowclosefun)(LWCGLFWwindow *window);
typedef void (*LWCGLFWwindowrefreshfun)(LWCGLFWwindow *window);
typedef void (*LWCGLFWwindowfocusfun)(LWCGLFWwindow *window, int focused);
typedef void (*LWCGLFWwindowiconifyfun)(LWCGLFWwindow *window, int iconified);
typedef void (*LWCGLFWwindowmaximizefun)(LWCGLFWwindow *window, int maximized);
typedef void (*LWCGLFWframebuffersizefun)(LWCGLFWwindow *window, int width, int height);
typedef void (*LWCGLFWwindowcontentscalefun)(LWCGLFWwindow *window, float xscale, float yscale);
typedef void (*LWCGLFWkeyfun)(LWCGLFWwindow *window, int key, int scancode, int action, int mods);
typedef void (*LWCGLFWcharfun)(LWCGLFWwindow *window, unsigned int codepoint);
typedef void (*LWCGLFWcharmodsfun)(LWCGLFWwindow *window, unsigned int codepoint, int mods);
typedef void (*LWCGLFWmousebuttonfun)(LWCGLFWwindow *window, int button, int action, int mods);
typedef void (*LWCGLFWcursorposfun)(LWCGLFWwindow *window, double xpos, double ypos);
typedef void (*LWCGLFWcursorenterfun)(LWCGLFWwindow *window, int entered);
typedef void (*LWCGLFWscrollfun)(LWCGLFWwindow *window, double xoffset, double yoffset);
typedef void (*LWCGLFWdropfun)(LWCGLFWwindow *window, int path_count, const char *paths[]);
typedef void (*LWCGLFWmonitorfun)(LWCGLFWmonitor *monitor, int event);
typedef void (*LWCGLFWjoystickfun)(int jid, int event);

typedef struct GLFWAPI {
    int GLFW_FALSE;
    int GLFW_TRUE;
    int GLFW_RELEASE;
    int GLFW_PRESS;
    int GLFW_REPEAT;

    int GLFW_KEY_UNKNOWN;
    int GLFW_KEY_SPACE;
    int GLFW_KEY_APOSTROPHE;
    int GLFW_KEY_COMMA;
    int GLFW_KEY_MINUS;
    int GLFW_KEY_PERIOD;
    int GLFW_KEY_SLASH;
    int GLFW_KEY_0;
    int GLFW_KEY_1;
    int GLFW_KEY_2;
    int GLFW_KEY_3;
    int GLFW_KEY_4;
    int GLFW_KEY_5;
    int GLFW_KEY_6;
    int GLFW_KEY_7;
    int GLFW_KEY_8;
    int GLFW_KEY_9;
    int GLFW_KEY_SEMICOLON;
    int GLFW_KEY_EQUAL;
    int GLFW_KEY_A;
    int GLFW_KEY_B;
    int GLFW_KEY_C;
    int GLFW_KEY_D;
    int GLFW_KEY_E;
    int GLFW_KEY_F;
    int GLFW_KEY_G;
    int GLFW_KEY_H;
    int GLFW_KEY_I;
    int GLFW_KEY_J;
    int GLFW_KEY_K;
    int GLFW_KEY_L;
    int GLFW_KEY_M;
    int GLFW_KEY_N;
    int GLFW_KEY_O;
    int GLFW_KEY_P;
    int GLFW_KEY_Q;
    int GLFW_KEY_R;
    int GLFW_KEY_S;
    int GLFW_KEY_T;
    int GLFW_KEY_U;
    int GLFW_KEY_V;
    int GLFW_KEY_W;
    int GLFW_KEY_X;
    int GLFW_KEY_Y;
    int GLFW_KEY_Z;
    int GLFW_KEY_LEFT_BRACKET;
    int GLFW_KEY_BACKSLASH;
    int GLFW_KEY_RIGHT_BRACKET;
    int GLFW_KEY_GRAVE_ACCENT;
    int GLFW_KEY_ESCAPE;
    int GLFW_KEY_ENTER;
    int GLFW_KEY_TAB;
    int GLFW_KEY_BACKSPACE;
    int GLFW_KEY_INSERT;
    int GLFW_KEY_DELETE;
    int GLFW_KEY_RIGHT;
    int GLFW_KEY_LEFT;
    int GLFW_KEY_DOWN;
    int GLFW_KEY_UP;
    int GLFW_KEY_PAGE_UP;
    int GLFW_KEY_PAGE_DOWN;
    int GLFW_KEY_HOME;
    int GLFW_KEY_END;
    int GLFW_KEY_CAPS_LOCK;
    int GLFW_KEY_SCROLL_LOCK;
    int GLFW_KEY_NUM_LOCK;
    int GLFW_KEY_PRINT_SCREEN;
    int GLFW_KEY_PAUSE;
    int GLFW_KEY_F1;
    int GLFW_KEY_F2;
    int GLFW_KEY_F3;
    int GLFW_KEY_F4;
    int GLFW_KEY_F5;
    int GLFW_KEY_F6;
    int GLFW_KEY_F7;
    int GLFW_KEY_F8;
    int GLFW_KEY_F9;
    int GLFW_KEY_F10;
    int GLFW_KEY_F11;
    int GLFW_KEY_F12;
    int GLFW_KEY_KP_0;
    int GLFW_KEY_KP_1;
    int GLFW_KEY_KP_2;
    int GLFW_KEY_KP_3;
    int GLFW_KEY_KP_4;
    int GLFW_KEY_KP_5;
    int GLFW_KEY_KP_6;
    int GLFW_KEY_KP_7;
    int GLFW_KEY_KP_8;
    int GLFW_KEY_KP_9;
    int GLFW_KEY_KP_DECIMAL;
    int GLFW_KEY_KP_DIVIDE;
    int GLFW_KEY_KP_MULTIPLY;
    int GLFW_KEY_KP_SUBTRACT;
    int GLFW_KEY_KP_ADD;
    int GLFW_KEY_KP_ENTER;
    int GLFW_KEY_KP_EQUAL;
    int GLFW_KEY_LEFT_SHIFT;
    int GLFW_KEY_LEFT_CONTROL;
    int GLFW_KEY_LEFT_ALT;
    int GLFW_KEY_LEFT_SUPER;
    int GLFW_KEY_RIGHT_SHIFT;
    int GLFW_KEY_RIGHT_CONTROL;
    int GLFW_KEY_RIGHT_ALT;
    int GLFW_KEY_RIGHT_SUPER;
    int GLFW_KEY_MENU;

    int GLFW_MOD_SHIFT;
    int GLFW_MOD_CONTROL;
    int GLFW_MOD_ALT;
    int GLFW_MOD_SUPER;
    int GLFW_MOD_CAPS_LOCK;
    int GLFW_MOD_NUM_LOCK;

    int GLFW_MOUSE_BUTTON_1;
    int GLFW_MOUSE_BUTTON_2;
    int GLFW_MOUSE_BUTTON_3;
    int GLFW_MOUSE_BUTTON_4;
    int GLFW_MOUSE_BUTTON_5;
    int GLFW_MOUSE_BUTTON_6;
    int GLFW_MOUSE_BUTTON_7;
    int GLFW_MOUSE_BUTTON_8;
    int GLFW_MOUSE_BUTTON_LEFT;
    int GLFW_MOUSE_BUTTON_RIGHT;
    int GLFW_MOUSE_BUTTON_MIDDLE;

    int GLFW_JOYSTICK_1;
    int GLFW_JOYSTICK_16;
    int GLFW_CONNECTED;
    int GLFW_DISCONNECTED;

    int GLFW_FOCUSED;
    int GLFW_ICONIFIED;
    int GLFW_RESIZABLE;
    int GLFW_VISIBLE;
    int GLFW_DECORATED;
    int GLFW_AUTO_ICONIFY;
    int GLFW_FLOATING;
    int GLFW_MAXIMIZED;
    int GLFW_CENTER_CURSOR;
    int GLFW_TRANSPARENT_FRAMEBUFFER;
    int GLFW_HOVERED;
    int GLFW_FOCUS_ON_SHOW;
    int GLFW_MOUSE_PASSTHROUGH;

    int GLFW_RED_BITS;
    int GLFW_GREEN_BITS;
    int GLFW_BLUE_BITS;
    int GLFW_ALPHA_BITS;
    int GLFW_DEPTH_BITS;
    int GLFW_STENCIL_BITS;
    int GLFW_ACCUM_RED_BITS;
    int GLFW_ACCUM_GREEN_BITS;
    int GLFW_ACCUM_BLUE_BITS;
    int GLFW_ACCUM_ALPHA_BITS;
    int GLFW_AUX_BUFFERS;
    int GLFW_STEREO;
    int GLFW_SAMPLES;
    int GLFW_SRGB_CAPABLE;
    int GLFW_REFRESH_RATE;
    int GLFW_DOUBLEBUFFER;

    int GLFW_CLIENT_API;
    int GLFW_CONTEXT_VERSION_MAJOR;
    int GLFW_CONTEXT_VERSION_MINOR;
    int GLFW_CONTEXT_REVISION;
    int GLFW_CONTEXT_ROBUSTNESS;
    int GLFW_OPENGL_FORWARD_COMPAT;
    int GLFW_OPENGL_DEBUG_CONTEXT;
    int GLFW_OPENGL_PROFILE;
    int GLFW_CONTEXT_RELEASE_BEHAVIOR;
    int GLFW_CONTEXT_NO_ERROR;
    int GLFW_CONTEXT_CREATION_API;
    int GLFW_SCALE_TO_MONITOR;

    int GLFW_NO_API;
    int GLFW_OPENGL_API;
    int GLFW_OPENGL_ES_API;
    int GLFW_OPENGL_ANY_PROFILE;
    int GLFW_OPENGL_CORE_PROFILE;
    int GLFW_OPENGL_COMPAT_PROFILE;

    int GLFW_CURSOR;
    int GLFW_STICKY_KEYS;
    int GLFW_STICKY_MOUSE_BUTTONS;
    int GLFW_LOCK_KEY_MODS;
    int GLFW_RAW_MOUSE_MOTION;
    int GLFW_CURSOR_NORMAL;
    int GLFW_CURSOR_HIDDEN;
    int GLFW_CURSOR_DISABLED;
    int GLFW_CURSOR_CAPTURED;

    int (*glfwInit)(void);
    void (*glfwTerminate)(void);
    void (*glfwInitHint)(int hint, int value);
    void (*glfwGetVersion)(int *major, int *minor, int *revision);
    const char *(*glfwGetVersionString)(void);
    LWCGLFWerrorfun (*glfwSetErrorCallback)(LWCGLFWerrorfun callback);

    LWCGLFWmonitor *(*glfwGetPrimaryMonitor)(void);
    LWCGLFWmonitorfun (*glfwSetMonitorCallback)(LWCGLFWmonitorfun callback);

    void (*glfwDefaultWindowHints)(void);
    void (*glfwWindowHint)(int hint, int value);
    void (*glfwWindowHintString)(int hint, const char *value);
    LWCGLFWwindow *(*glfwCreateWindow)(int width, int height, const char *title,
                                      LWCGLFWmonitor *monitor, LWCGLFWwindow *share);
    void (*glfwDestroyWindow)(LWCGLFWwindow *window);
    int (*glfwWindowShouldClose)(LWCGLFWwindow *window);
    void (*glfwSetWindowShouldClose)(LWCGLFWwindow *window, int value);
    void (*glfwSetWindowTitle)(LWCGLFWwindow *window, const char *title);
    void (*glfwGetWindowPos)(LWCGLFWwindow *window, int *xpos, int *ypos);
    void (*glfwSetWindowPos)(LWCGLFWwindow *window, int xpos, int ypos);
    void (*glfwGetWindowSize)(LWCGLFWwindow *window, int *width, int *height);
    void (*glfwSetWindowSize)(LWCGLFWwindow *window, int width, int height);
    void (*glfwGetFramebufferSize)(LWCGLFWwindow *window, int *width, int *height);
    void (*glfwGetWindowContentScale)(LWCGLFWwindow *window, float *xscale, float *yscale);
    float (*glfwGetWindowOpacity)(LWCGLFWwindow *window);
    void (*glfwSetWindowOpacity)(LWCGLFWwindow *window, float opacity);
    void (*glfwIconifyWindow)(LWCGLFWwindow *window);
    void (*glfwRestoreWindow)(LWCGLFWwindow *window);
    void (*glfwMaximizeWindow)(LWCGLFWwindow *window);
    void (*glfwShowWindow)(LWCGLFWwindow *window);
    void (*glfwHideWindow)(LWCGLFWwindow *window);
    void (*glfwFocusWindow)(LWCGLFWwindow *window);
    void (*glfwRequestWindowAttention)(LWCGLFWwindow *window);
    LWCGLFWmonitor *(*glfwGetWindowMonitor)(LWCGLFWwindow *window);
    void (*glfwSetWindowMonitor)(LWCGLFWwindow *window, LWCGLFWmonitor *monitor,
                                 int xpos, int ypos, int width, int height, int refresh_rate);
    int (*glfwGetWindowAttrib)(LWCGLFWwindow *window, int attrib);
    void (*glfwSetWindowAttrib)(LWCGLFWwindow *window, int attrib, int value);
    void (*glfwSetWindowUserPointer)(LWCGLFWwindow *window, void *pointer);
    void *(*glfwGetWindowUserPointer)(LWCGLFWwindow *window);

    LWCGLFWwindowposfun (*glfwSetWindowPosCallback)(LWCGLFWwindow *, LWCGLFWwindowposfun);
    LWCGLFWwindowsizefun (*glfwSetWindowSizeCallback)(LWCGLFWwindow *, LWCGLFWwindowsizefun);
    LWCGLFWwindowclosefun (*glfwSetWindowCloseCallback)(LWCGLFWwindow *, LWCGLFWwindowclosefun);
    LWCGLFWwindowrefreshfun (*glfwSetWindowRefreshCallback)(LWCGLFWwindow *, LWCGLFWwindowrefreshfun);
    LWCGLFWwindowfocusfun (*glfwSetWindowFocusCallback)(LWCGLFWwindow *, LWCGLFWwindowfocusfun);
    LWCGLFWwindowiconifyfun (*glfwSetWindowIconifyCallback)(LWCGLFWwindow *, LWCGLFWwindowiconifyfun);
    LWCGLFWwindowmaximizefun (*glfwSetWindowMaximizeCallback)(LWCGLFWwindow *, LWCGLFWwindowmaximizefun);
    LWCGLFWframebuffersizefun (*glfwSetFramebufferSizeCallback)(LWCGLFWwindow *, LWCGLFWframebuffersizefun);
    LWCGLFWwindowcontentscalefun (*glfwSetWindowContentScaleCallback)(LWCGLFWwindow *, LWCGLFWwindowcontentscalefun);

    void (*glfwPollEvents)(void);
    void (*glfwWaitEvents)(void);
    void (*glfwWaitEventsTimeout)(double timeout);
    void (*glfwPostEmptyEvent)(void);

    int (*glfwGetInputMode)(LWCGLFWwindow *window, int mode);
    void (*glfwSetInputMode)(LWCGLFWwindow *window, int mode, int value);
    int (*glfwRawMouseMotionSupported)(void);
    const char *(*glfwGetKeyName)(int key, int scancode);
    int (*glfwGetKeyScancode)(int key);
    int (*glfwGetKey)(LWCGLFWwindow *window, int key);
    int (*glfwGetMouseButton)(LWCGLFWwindow *window, int button);
    void (*glfwGetCursorPos)(LWCGLFWwindow *window, double *xpos, double *ypos);
    void (*glfwSetCursorPos)(LWCGLFWwindow *window, double xpos, double ypos);
    LWCGLFWkeyfun (*glfwSetKeyCallback)(LWCGLFWwindow *, LWCGLFWkeyfun);
    LWCGLFWcharfun (*glfwSetCharCallback)(LWCGLFWwindow *, LWCGLFWcharfun);
    LWCGLFWcharmodsfun (*glfwSetCharModsCallback)(LWCGLFWwindow *, LWCGLFWcharmodsfun);
    LWCGLFWmousebuttonfun (*glfwSetMouseButtonCallback)(LWCGLFWwindow *, LWCGLFWmousebuttonfun);
    LWCGLFWcursorposfun (*glfwSetCursorPosCallback)(LWCGLFWwindow *, LWCGLFWcursorposfun);
    LWCGLFWcursorenterfun (*glfwSetCursorEnterCallback)(LWCGLFWwindow *, LWCGLFWcursorenterfun);
    LWCGLFWscrollfun (*glfwSetScrollCallback)(LWCGLFWwindow *, LWCGLFWscrollfun);
    LWCGLFWdropfun (*glfwSetDropCallback)(LWCGLFWwindow *, LWCGLFWdropfun);

    int (*glfwJoystickPresent)(int jid);
    const float *(*glfwGetJoystickAxes)(int jid, int *count);
    const unsigned char *(*glfwGetJoystickButtons)(int jid, int *count);
    const unsigned char *(*glfwGetJoystickHats)(int jid, int *count);
    const char *(*glfwGetJoystickName)(int jid);
    const char *(*glfwGetJoystickGUID)(int jid);
    void (*glfwSetJoystickUserPointer)(int jid, void *pointer);
    void *(*glfwGetJoystickUserPointer)(int jid);
    int (*glfwJoystickIsGamepad)(int jid);
    LWCGLFWjoystickfun (*glfwSetJoystickCallback)(LWCGLFWjoystickfun callback);
    int (*glfwUpdateGamepadMappings)(const char *mapping);
    const char *(*glfwGetGamepadName)(int jid);

    void (*glfwSetClipboardString)(LWCGLFWwindow *window, const char *string);
    const char *(*glfwGetClipboardString)(LWCGLFWwindow *window);
    double (*glfwGetTime)(void);
    void (*glfwSetTime)(double time);
    uint64_t (*glfwGetTimerValue)(void);
    uint64_t (*glfwGetTimerFrequency)(void);

    void (*glfwMakeContextCurrent)(LWCGLFWwindow *window);
    LWCGLFWwindow *(*glfwGetCurrentContext)(void);
    void (*glfwSwapBuffers)(LWCGLFWwindow *window);
    void (*glfwSwapInterval)(int interval);
    int (*glfwExtensionSupported)(const char *extension);
    LWCGLFWglproc (*glfwGetProcAddress)(const char *procname);
} GLFWAPI;

extern GLFWAPI GLFW;

#ifdef __cplusplus
}
#endif

#endif
