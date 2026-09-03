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
    if (key == GLFW_KEY_ESCAPE)
    {
        lwcgl_input_set_key(
                &g_fast_input,
                Keyboard.KEY_ESCAPE,
                action != GLFW_RELEASE
            );
    }

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
            const int state = glfwGetKey(window, GLFW_KEY_ESCAPE);
            lwcgl_input_set_key(
                    &g_fast_input,
                    Keyboard.KEY_ESCAPE,
                    state == GLFW_PRESS || state == GLFW_REPEAT
                );

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
    lwcgl_input_set_key(&g_fast_input, Keyboard.KEY_ESCAPE, 0);
}

static LWCGLbool fast_keyboard_is_down(int key)
{
    if (key == Keyboard.KEY_ESCAPE)
    {
        return lwcgl_input_key_down(&g_fast_input, key)
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
