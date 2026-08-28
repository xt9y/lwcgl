#include <lwcgl/lwcgl.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

/* GLFW uses GLFWAPI as an export/import macro. lwcgl uses it as the public
 * table type name, so discard the native header macro after its declarations
 * have been parsed. */
#ifdef GLFWAPI
#undef GLFWAPI
#endif

/*
 * Keep this positional. The public API intentionally uses LWJGL-style names
 * such as GLFW.GLFW_TRUE; including glfw3.h here turns those names into C
 * preprocessor macros and would make designated initializers impossible.
 */
GLFWAPI GLFW = {
    GLFW_FALSE, GLFW_TRUE, GLFW_RELEASE, GLFW_PRESS, GLFW_REPEAT,

    GLFW_KEY_UNKNOWN, GLFW_KEY_SPACE, GLFW_KEY_APOSTROPHE, GLFW_KEY_COMMA,
    GLFW_KEY_MINUS, GLFW_KEY_PERIOD, GLFW_KEY_SLASH,
    GLFW_KEY_0, GLFW_KEY_1, GLFW_KEY_2, GLFW_KEY_3, GLFW_KEY_4,
    GLFW_KEY_5, GLFW_KEY_6, GLFW_KEY_7, GLFW_KEY_8, GLFW_KEY_9,
    GLFW_KEY_SEMICOLON, GLFW_KEY_EQUAL,
    GLFW_KEY_A, GLFW_KEY_B, GLFW_KEY_C, GLFW_KEY_D, GLFW_KEY_E, GLFW_KEY_F,
    GLFW_KEY_G, GLFW_KEY_H, GLFW_KEY_I, GLFW_KEY_J, GLFW_KEY_K, GLFW_KEY_L,
    GLFW_KEY_M, GLFW_KEY_N, GLFW_KEY_O, GLFW_KEY_P, GLFW_KEY_Q, GLFW_KEY_R,
    GLFW_KEY_S, GLFW_KEY_T, GLFW_KEY_U, GLFW_KEY_V, GLFW_KEY_W, GLFW_KEY_X,
    GLFW_KEY_Y, GLFW_KEY_Z,
    GLFW_KEY_LEFT_BRACKET, GLFW_KEY_BACKSLASH, GLFW_KEY_RIGHT_BRACKET,
    GLFW_KEY_GRAVE_ACCENT, GLFW_KEY_ESCAPE, GLFW_KEY_ENTER, GLFW_KEY_TAB,
    GLFW_KEY_BACKSPACE, GLFW_KEY_INSERT, GLFW_KEY_DELETE, GLFW_KEY_RIGHT,
    GLFW_KEY_LEFT, GLFW_KEY_DOWN, GLFW_KEY_UP, GLFW_KEY_PAGE_UP,
    GLFW_KEY_PAGE_DOWN, GLFW_KEY_HOME, GLFW_KEY_END, GLFW_KEY_CAPS_LOCK,
    GLFW_KEY_SCROLL_LOCK, GLFW_KEY_NUM_LOCK, GLFW_KEY_PRINT_SCREEN,
    GLFW_KEY_PAUSE, GLFW_KEY_F1, GLFW_KEY_F2, GLFW_KEY_F3, GLFW_KEY_F4,
    GLFW_KEY_F5, GLFW_KEY_F6, GLFW_KEY_F7, GLFW_KEY_F8, GLFW_KEY_F9,
    GLFW_KEY_F10, GLFW_KEY_F11, GLFW_KEY_F12,
    GLFW_KEY_KP_0, GLFW_KEY_KP_1, GLFW_KEY_KP_2, GLFW_KEY_KP_3,
    GLFW_KEY_KP_4, GLFW_KEY_KP_5, GLFW_KEY_KP_6, GLFW_KEY_KP_7,
    GLFW_KEY_KP_8, GLFW_KEY_KP_9, GLFW_KEY_KP_DECIMAL, GLFW_KEY_KP_DIVIDE,
    GLFW_KEY_KP_MULTIPLY, GLFW_KEY_KP_SUBTRACT, GLFW_KEY_KP_ADD,
    GLFW_KEY_KP_ENTER, GLFW_KEY_KP_EQUAL,
    GLFW_KEY_LEFT_SHIFT, GLFW_KEY_LEFT_CONTROL, GLFW_KEY_LEFT_ALT,
    GLFW_KEY_LEFT_SUPER, GLFW_KEY_RIGHT_SHIFT, GLFW_KEY_RIGHT_CONTROL,
    GLFW_KEY_RIGHT_ALT, GLFW_KEY_RIGHT_SUPER, GLFW_KEY_MENU,

    GLFW_MOD_SHIFT, GLFW_MOD_CONTROL, GLFW_MOD_ALT, GLFW_MOD_SUPER,
    GLFW_MOD_CAPS_LOCK, GLFW_MOD_NUM_LOCK,

    GLFW_MOUSE_BUTTON_1, GLFW_MOUSE_BUTTON_2, GLFW_MOUSE_BUTTON_3,
    GLFW_MOUSE_BUTTON_4, GLFW_MOUSE_BUTTON_5, GLFW_MOUSE_BUTTON_6,
    GLFW_MOUSE_BUTTON_7, GLFW_MOUSE_BUTTON_8, GLFW_MOUSE_BUTTON_LEFT,
    GLFW_MOUSE_BUTTON_RIGHT, GLFW_MOUSE_BUTTON_MIDDLE,

    GLFW_JOYSTICK_1, GLFW_JOYSTICK_16, GLFW_CONNECTED, GLFW_DISCONNECTED,

    GLFW_FOCUSED, GLFW_ICONIFIED, GLFW_RESIZABLE, GLFW_VISIBLE,
    GLFW_DECORATED, GLFW_AUTO_ICONIFY, GLFW_FLOATING, GLFW_MAXIMIZED,
    GLFW_CENTER_CURSOR, GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_HOVERED,
    GLFW_FOCUS_ON_SHOW,
#ifdef GLFW_MOUSE_PASSTHROUGH
    GLFW_MOUSE_PASSTHROUGH,
#else
    0x0002000D,
#endif

    GLFW_RED_BITS, GLFW_GREEN_BITS, GLFW_BLUE_BITS, GLFW_ALPHA_BITS,
    GLFW_DEPTH_BITS, GLFW_STENCIL_BITS, GLFW_ACCUM_RED_BITS,
    GLFW_ACCUM_GREEN_BITS, GLFW_ACCUM_BLUE_BITS, GLFW_ACCUM_ALPHA_BITS,
    GLFW_AUX_BUFFERS, GLFW_STEREO, GLFW_SAMPLES, GLFW_SRGB_CAPABLE,
    GLFW_REFRESH_RATE, GLFW_DOUBLEBUFFER,

    GLFW_CLIENT_API, GLFW_CONTEXT_VERSION_MAJOR, GLFW_CONTEXT_VERSION_MINOR,
    GLFW_CONTEXT_REVISION, GLFW_CONTEXT_ROBUSTNESS, GLFW_OPENGL_FORWARD_COMPAT,
    GLFW_OPENGL_DEBUG_CONTEXT, GLFW_OPENGL_PROFILE,
    GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_CONTEXT_NO_ERROR,
    GLFW_CONTEXT_CREATION_API,
#ifdef GLFW_SCALE_TO_MONITOR
    GLFW_SCALE_TO_MONITOR,
#else
    0x0002200C,
#endif

    GLFW_NO_API, GLFW_OPENGL_API, GLFW_OPENGL_ES_API,
    GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_CORE_PROFILE,
    GLFW_OPENGL_COMPAT_PROFILE,

    GLFW_CURSOR, GLFW_STICKY_KEYS, GLFW_STICKY_MOUSE_BUTTONS,
    GLFW_LOCK_KEY_MODS, GLFW_RAW_MOUSE_MOTION,
    GLFW_CURSOR_NORMAL, GLFW_CURSOR_HIDDEN, GLFW_CURSOR_DISABLED,
#ifdef GLFW_CURSOR_CAPTURED
    GLFW_CURSOR_CAPTURED,
#else
    0x00034004,
#endif

    glfwInit,
    glfwTerminate,
    glfwInitHint,
    glfwGetVersion,
    glfwGetVersionString,
    (LWCGLFWerrorfun (*)(LWCGLFWerrorfun))glfwSetErrorCallback,

    (LWCGLFWmonitor *(*)(void))glfwGetPrimaryMonitor,
    (LWCGLFWmonitorfun (*)(LWCGLFWmonitorfun))glfwSetMonitorCallback,

    glfwDefaultWindowHints,
    glfwWindowHint,
    glfwWindowHintString,
    (LWCGLFWwindow *(*)(int, int, const char *, LWCGLFWmonitor *, LWCGLFWwindow *))glfwCreateWindow,
    (void (*)(LWCGLFWwindow *))glfwDestroyWindow,
    (int (*)(LWCGLFWwindow *))glfwWindowShouldClose,
    (void (*)(LWCGLFWwindow *, int))glfwSetWindowShouldClose,
    (void (*)(LWCGLFWwindow *, const char *))glfwSetWindowTitle,
    (void (*)(LWCGLFWwindow *, int *, int *))glfwGetWindowPos,
    (void (*)(LWCGLFWwindow *, int, int))glfwSetWindowPos,
    (void (*)(LWCGLFWwindow *, int *, int *))glfwGetWindowSize,
    (void (*)(LWCGLFWwindow *, int, int))glfwSetWindowSize,
    (void (*)(LWCGLFWwindow *, int *, int *))glfwGetFramebufferSize,
    (void (*)(LWCGLFWwindow *, float *, float *))glfwGetWindowContentScale,
    (float (*)(LWCGLFWwindow *))glfwGetWindowOpacity,
    (void (*)(LWCGLFWwindow *, float))glfwSetWindowOpacity,
    (void (*)(LWCGLFWwindow *))glfwIconifyWindow,
    (void (*)(LWCGLFWwindow *))glfwRestoreWindow,
    (void (*)(LWCGLFWwindow *))glfwMaximizeWindow,
    (void (*)(LWCGLFWwindow *))glfwShowWindow,
    (void (*)(LWCGLFWwindow *))glfwHideWindow,
    (void (*)(LWCGLFWwindow *))glfwFocusWindow,
    (void (*)(LWCGLFWwindow *))glfwRequestWindowAttention,
    (LWCGLFWmonitor *(*)(LWCGLFWwindow *))glfwGetWindowMonitor,
    (void (*)(LWCGLFWwindow *, LWCGLFWmonitor *, int, int, int, int, int))glfwSetWindowMonitor,
    (int (*)(LWCGLFWwindow *, int))glfwGetWindowAttrib,
    (void (*)(LWCGLFWwindow *, int, int))glfwSetWindowAttrib,
    (void (*)(LWCGLFWwindow *, void *))glfwSetWindowUserPointer,
    (void *(*)(LWCGLFWwindow *))glfwGetWindowUserPointer,

    (LWCGLFWwindowposfun (*)(LWCGLFWwindow *, LWCGLFWwindowposfun))glfwSetWindowPosCallback,
    (LWCGLFWwindowsizefun (*)(LWCGLFWwindow *, LWCGLFWwindowsizefun))glfwSetWindowSizeCallback,
    (LWCGLFWwindowclosefun (*)(LWCGLFWwindow *, LWCGLFWwindowclosefun))glfwSetWindowCloseCallback,
    (LWCGLFWwindowrefreshfun (*)(LWCGLFWwindow *, LWCGLFWwindowrefreshfun))glfwSetWindowRefreshCallback,
    (LWCGLFWwindowfocusfun (*)(LWCGLFWwindow *, LWCGLFWwindowfocusfun))glfwSetWindowFocusCallback,
    (LWCGLFWwindowiconifyfun (*)(LWCGLFWwindow *, LWCGLFWwindowiconifyfun))glfwSetWindowIconifyCallback,
    (LWCGLFWwindowmaximizefun (*)(LWCGLFWwindow *, LWCGLFWwindowmaximizefun))glfwSetWindowMaximizeCallback,
    (LWCGLFWframebuffersizefun (*)(LWCGLFWwindow *, LWCGLFWframebuffersizefun))glfwSetFramebufferSizeCallback,
    (LWCGLFWwindowcontentscalefun (*)(LWCGLFWwindow *, LWCGLFWwindowcontentscalefun))glfwSetWindowContentScaleCallback,

    glfwPollEvents,
    glfwWaitEvents,
    glfwWaitEventsTimeout,
    glfwPostEmptyEvent,

    (int (*)(LWCGLFWwindow *, int))glfwGetInputMode,
    (void (*)(LWCGLFWwindow *, int, int))glfwSetInputMode,
    glfwRawMouseMotionSupported,
    glfwGetKeyName,
    glfwGetKeyScancode,
    (int (*)(LWCGLFWwindow *, int))glfwGetKey,
    (int (*)(LWCGLFWwindow *, int))glfwGetMouseButton,
    (void (*)(LWCGLFWwindow *, double *, double *))glfwGetCursorPos,
    (void (*)(LWCGLFWwindow *, double, double))glfwSetCursorPos,
    (LWCGLFWkeyfun (*)(LWCGLFWwindow *, LWCGLFWkeyfun))glfwSetKeyCallback,
    (LWCGLFWcharfun (*)(LWCGLFWwindow *, LWCGLFWcharfun))glfwSetCharCallback,
    (LWCGLFWcharmodsfun (*)(LWCGLFWwindow *, LWCGLFWcharmodsfun))glfwSetCharModsCallback,
    (LWCGLFWmousebuttonfun (*)(LWCGLFWwindow *, LWCGLFWmousebuttonfun))glfwSetMouseButtonCallback,
    (LWCGLFWcursorposfun (*)(LWCGLFWwindow *, LWCGLFWcursorposfun))glfwSetCursorPosCallback,
    (LWCGLFWcursorenterfun (*)(LWCGLFWwindow *, LWCGLFWcursorenterfun))glfwSetCursorEnterCallback,
    (LWCGLFWscrollfun (*)(LWCGLFWwindow *, LWCGLFWscrollfun))glfwSetScrollCallback,
    (LWCGLFWdropfun (*)(LWCGLFWwindow *, LWCGLFWdropfun))glfwSetDropCallback,

    glfwJoystickPresent,
    glfwGetJoystickAxes,
    glfwGetJoystickButtons,
    glfwGetJoystickHats,
    glfwGetJoystickName,
    glfwGetJoystickGUID,
    glfwSetJoystickUserPointer,
    glfwGetJoystickUserPointer,
    glfwJoystickIsGamepad,
    (LWCGLFWjoystickfun (*)(LWCGLFWjoystickfun))glfwSetJoystickCallback,
    glfwUpdateGamepadMappings,
    glfwGetGamepadName,

    (void (*)(LWCGLFWwindow *, const char *))glfwSetClipboardString,
    (const char *(*)(LWCGLFWwindow *))glfwGetClipboardString,
    glfwGetTime,
    glfwSetTime,
    glfwGetTimerValue,
    glfwGetTimerFrequency,

    (void (*)(LWCGLFWwindow *))glfwMakeContextCurrent,
    (LWCGLFWwindow *(*)(void))glfwGetCurrentContext,
    (void (*)(LWCGLFWwindow *))glfwSwapBuffers,
    glfwSwapInterval,
    glfwExtensionSupported,
    (LWCGLFWglproc (*)(const char *))glfwGetProcAddress
};
