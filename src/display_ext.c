#include <lwcgl/lwcgl.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "input_state.h"

static LwcglInputState g_fast_input;
static GLFWwindow *g_fast_window;
static GLFWkeyfun g_chained_key_callback;
static GLFWwindowclosefun g_chained_close_callback;

static int (*g_base_display_create)(void);
static void (*g_base_display_destroy)(void);
static int (*g_base_keyboard_create)(void);
static void (*g_base_keyboard_destroy)(void);
static LWCGLbool (*g_base_keyboard_is_down)(int key);
static LWCGLbool g_fast_runtime_installed;

static int fast_lwjgl_to_glfw_key(int k)
{
    switch (k) {
        case 1: return GLFW_KEY_ESCAPE;
        case 2: return GLFW_KEY_1; case 3: return GLFW_KEY_2; case 4: return GLFW_KEY_3;
        case 5: return GLFW_KEY_4; case 6: return GLFW_KEY_5; case 7: return GLFW_KEY_6;
        case 8: return GLFW_KEY_7; case 9: return GLFW_KEY_8; case 10: return GLFW_KEY_9;
        case 11: return GLFW_KEY_0; case 12: return GLFW_KEY_MINUS; case 13: return GLFW_KEY_EQUAL;
        case 14: return GLFW_KEY_BACKSPACE; case 15: return GLFW_KEY_TAB;
        case 16: return GLFW_KEY_Q; case 17: return GLFW_KEY_W; case 18: return GLFW_KEY_E;
        case 19: return GLFW_KEY_R; case 20: return GLFW_KEY_T; case 21: return GLFW_KEY_Y;
        case 22: return GLFW_KEY_U; case 23: return GLFW_KEY_I; case 24: return GLFW_KEY_O;
        case 25: return GLFW_KEY_P; case 26: return GLFW_KEY_LEFT_BRACKET; case 27: return GLFW_KEY_RIGHT_BRACKET;
        case 28: return GLFW_KEY_ENTER; case 29: return GLFW_KEY_LEFT_CONTROL;
        case 30: return GLFW_KEY_A; case 31: return GLFW_KEY_S; case 32: return GLFW_KEY_D;
        case 33: return GLFW_KEY_F; case 34: return GLFW_KEY_G; case 35: return GLFW_KEY_H;
        case 36: return GLFW_KEY_J; case 37: return GLFW_KEY_K; case 38: return GLFW_KEY_L;
        case 39: return GLFW_KEY_SEMICOLON; case 40: return GLFW_KEY_APOSTROPHE; case 41: return GLFW_KEY_GRAVE_ACCENT;
        case 42: return GLFW_KEY_LEFT_SHIFT; case 43: return GLFW_KEY_BACKSLASH;
        case 44: return GLFW_KEY_Z; case 45: return GLFW_KEY_X; case 46: return GLFW_KEY_C;
        case 47: return GLFW_KEY_V; case 48: return GLFW_KEY_B; case 49: return GLFW_KEY_N;
        case 50: return GLFW_KEY_M; case 51: return GLFW_KEY_COMMA; case 52: return GLFW_KEY_PERIOD;
        case 53: return GLFW_KEY_SLASH; case 54: return GLFW_KEY_RIGHT_SHIFT;
        case 55: return GLFW_KEY_KP_MULTIPLY; case 56: return GLFW_KEY_LEFT_ALT; case 57: return GLFW_KEY_SPACE;
        case 58: return GLFW_KEY_CAPS_LOCK; case 59: return GLFW_KEY_F1; case 60: return GLFW_KEY_F2;
        case 61: return GLFW_KEY_F3; case 62: return GLFW_KEY_F4; case 63: return GLFW_KEY_F5;
        case 64: return GLFW_KEY_F6; case 65: return GLFW_KEY_F7; case 66: return GLFW_KEY_F8;
        case 67: return GLFW_KEY_F9; case 68: return GLFW_KEY_F10; case 69: return GLFW_KEY_NUM_LOCK;
        case 70: return GLFW_KEY_SCROLL_LOCK; case 71: return GLFW_KEY_KP_7; case 72: return GLFW_KEY_KP_8;
        case 73: return GLFW_KEY_KP_9; case 74: return GLFW_KEY_KP_SUBTRACT; case 75: return GLFW_KEY_KP_4;
        case 76: return GLFW_KEY_KP_5; case 77: return GLFW_KEY_KP_6; case 78: return GLFW_KEY_KP_ADD;
        case 79: return GLFW_KEY_KP_1; case 80: return GLFW_KEY_KP_2; case 81: return GLFW_KEY_KP_3;
        case 82: return GLFW_KEY_KP_0; case 83: return GLFW_KEY_KP_DECIMAL;
        case 87: return GLFW_KEY_F11; case 88: return GLFW_KEY_F12;
        case 100: return GLFW_KEY_F13; case 101: return GLFW_KEY_F14; case 102: return GLFW_KEY_F15;
        case 141: return GLFW_KEY_KP_EQUAL; case 156: return GLFW_KEY_KP_ENTER;
        case 157: return GLFW_KEY_RIGHT_CONTROL; case 181: return GLFW_KEY_KP_DIVIDE;
        case 183: return GLFW_KEY_PRINT_SCREEN; case 184: return GLFW_KEY_RIGHT_ALT;
        case 197: return GLFW_KEY_PAUSE; case 199: return GLFW_KEY_HOME; case 200: return GLFW_KEY_UP;
        case 201: return GLFW_KEY_PAGE_UP; case 203: return GLFW_KEY_LEFT; case 205: return GLFW_KEY_RIGHT;
        case 207: return GLFW_KEY_END; case 208: return GLFW_KEY_DOWN; case 209: return GLFW_KEY_PAGE_DOWN;
        case 210: return GLFW_KEY_INSERT; case 211: return GLFW_KEY_DELETE;
        case 219: return GLFW_KEY_LEFT_SUPER; case 220: return GLFW_KEY_RIGHT_SUPER; case 221: return GLFW_KEY_MENU;
        default: return GLFW_KEY_UNKNOWN;
    }
}

static void fast_close_callback(GLFWwindow *window)
{
    lwcgl_input_request_close(&g_fast_input);

    if (g_chained_close_callback)
    {
        g_chained_close_callback(window);
    }
}

static void fast_key_callback(
        GLFWwindow *window,
        int key,
        int scancode,
        int action,
        int mods)
{
    lwcgl_input_set_key(
            &g_fast_input,
            key,
            action != GLFW_RELEASE
        );

    if (g_chained_key_callback)
    {
        g_chained_key_callback(window, key, scancode, action, mods);
    }
}

static int fast_display_create(void)
{
    const int result = g_base_display_create ? g_base_display_create() : -1;

    if (result == 0)
    {
        lwcgl_input_reset(&g_fast_input);
        g_fast_window = (GLFWwindow *)Display.getNativeWindow();

        if (g_fast_window)
        {
            g_chained_close_callback = glfwSetWindowCloseCallback(
                    g_fast_window,
                    fast_close_callback
                );
        }
    }

    return result;
}

static void fast_display_destroy(void)
{
    if (g_base_display_destroy)
    {
        g_base_display_destroy();
    }

    g_fast_window = NULL;
    g_chained_close_callback = NULL;
    g_chained_key_callback = NULL;
    lwcgl_input_reset(&g_fast_input);
}

static LWCGLbool fast_display_close_requested(void)
{
    return lwcgl_input_close_requested(&g_fast_input)
        ? LWCGL_TRUE
        : LWCGL_FALSE;
}

static int fast_keyboard_create(void)
{
    const int result = g_base_keyboard_create ? g_base_keyboard_create() : -1;

    if (result == 0)
    {
        GLFWwindow *window = g_fast_window
            ? g_fast_window
            : (GLFWwindow *)Display.getNativeWindow();

        if (window)
        {
            for (int lwjgl_key = 1; lwjgl_key < 256; ++lwjgl_key)
            {
                const int glfw_key = fast_lwjgl_to_glfw_key(lwjgl_key);

                if (glfw_key == GLFW_KEY_UNKNOWN)
                {
                    continue;
                }

                const int state = glfwGetKey(window, glfw_key);
                lwcgl_input_set_key(
                        &g_fast_input,
                        glfw_key,
                        state == GLFW_PRESS || state == GLFW_REPEAT
                    );
            }

            g_chained_key_callback = glfwSetKeyCallback(
                    window,
                    fast_key_callback
                );
        }
    }

    return result;
}

static void fast_keyboard_destroy(void)
{
    if (g_base_keyboard_destroy)
    {
        g_base_keyboard_destroy();
    }

    g_chained_key_callback = NULL;
    for (int key = 0; key < LWCGL_INPUT_KEY_CAPACITY; ++key)
    {
        g_fast_input.key_down[key] = 0u;
    }
}

static LWCGLbool fast_keyboard_is_down(int key)
{
    const int glfw_key = fast_lwjgl_to_glfw_key(key);

    if (glfw_key != GLFW_KEY_UNKNOWN)
    {
        return lwcgl_input_key_down(&g_fast_input, glfw_key)
            ? LWCGL_TRUE
            : LWCGL_FALSE;
    }

    return g_base_keyboard_is_down
        ? g_base_keyboard_is_down(key)
        : LWCGL_FALSE;
}

static void display_update_no_messages(void)
{
    if (!g_fast_window)
    {
        g_fast_window = (GLFWwindow *)Display.getNativeWindow();
    }

    if (g_fast_window)
    {
        glfwSwapBuffers(g_fast_window);
    }
}

void lwcglInstallFastRuntime(void)
{
    if (g_fast_runtime_installed)
    {
        return;
    }

    g_base_display_create = Display.create;
    g_base_display_destroy = Display.destroy;
    g_base_keyboard_create = Keyboard.create;
    g_base_keyboard_destroy = Keyboard.destroy;
    g_base_keyboard_is_down = Keyboard.isKeyDown;

    Display.create = fast_display_create;
    Display.destroy = fast_display_destroy;
    Display.isCloseRequested = fast_display_close_requested;
    Display.updateNoMessages = display_update_no_messages;

    Keyboard.create = fast_keyboard_create;
    Keyboard.destroy = fast_keyboard_destroy;
    Keyboard.isKeyDown = fast_keyboard_is_down;

    g_fast_runtime_installed = LWCGL_TRUE;
}

void lwcglDisplayUpdateNoMessages(void)
{
    if (Display.updateNoMessages)
    {
        Display.updateNoMessages();
        return;
    }

    display_update_no_messages();
}
