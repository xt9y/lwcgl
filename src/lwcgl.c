#include <lwcgl/lwcgl.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LWCGL_EVENT_CAPACITY 1024
#define LWCGL_MOUSE_BUTTONS 16

static char g_error[512];
static GLFWwindow *g_window;
static DisplayMode g_display_mode = {640, 480, 0, 0, LWCGL_FALSE};
static char g_title[256] = "Game";
static LWCGLbool g_created;
static LWCGLbool g_resizable = LWCGL_TRUE;
static LWCGLbool g_vsync;
static LWCGLbool g_fullscreen;
static int g_windowed_x = 100;
static int g_windowed_y = 100;
static int g_windowed_w = 640;
static int g_windowed_h = 480;

static void set_error(const char *message) {
    if (!message) message = "unknown error";
    snprintf(g_error, sizeof(g_error), "%s", message);
}

const char *lwcglGetLastError(void) {
    return g_error[0] ? g_error : NULL;
}

void lwcglClearError(void) {
    g_error[0] = '\0';
}

static uint64_t nano_time(void) {
#if defined(CLOCK_MONOTONIC)
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
        return (uint64_t)ts.tv_sec * 1000000000ull + (uint64_t)ts.tv_nsec;
#endif
    return (uint64_t)(glfwGetTime() * 1000000000.0);
}

/* ------------------------------------------------------------------------- */
/* BufferUtils / Buffer                                                      */
/* ------------------------------------------------------------------------- */

static LWCGLBuffer *buffer_create(size_t count, size_t element_size,
                                  LWCGLBufferScalarType scalar_type) {
    LWCGLBuffer *buffer = (LWCGLBuffer *)calloc(1, sizeof(*buffer));
    if (!buffer) {
        set_error("out of memory allocating buffer object");
        return NULL;
    }

    if (count && element_size > SIZE_MAX / count) {
        free(buffer);
        set_error("buffer size overflow");
        return NULL;
    }

    buffer->data = calloc(count ? count : 1, element_size);
    if (!buffer->data) {
        free(buffer);
        set_error("out of memory allocating buffer storage");
        return NULL;
    }

    buffer->elementSize = element_size;
    buffer->capacity = count;
    buffer->position = 0;
    buffer->limit = count;
    buffer->mark = SIZE_MAX;
    buffer->scalarType = scalar_type;
    return buffer;
}

static ByteBuffer *buffer_create_byte(size_t n) {
    return buffer_create(n, sizeof(int8_t), LWCGL_BUFFER_BYTE);
}
static ShortBuffer *buffer_create_short(size_t n) {
    return buffer_create(n, sizeof(int16_t), LWCGL_BUFFER_SHORT);
}
static IntBuffer *buffer_create_int(size_t n) {
    return buffer_create(n, sizeof(int32_t), LWCGL_BUFFER_INT);
}
static LongBuffer *buffer_create_long(size_t n) {
    return buffer_create(n, sizeof(int64_t), LWCGL_BUFFER_LONG);
}
static FloatBuffer *buffer_create_float(size_t n) {
    return buffer_create(n, sizeof(float), LWCGL_BUFFER_FLOAT);
}
static DoubleBuffer *buffer_create_double(size_t n) {
    return buffer_create(n, sizeof(double), LWCGL_BUFFER_DOUBLE);
}

static void buffer_destroy(LWCGLBuffer *buffer) {
    if (!buffer) return;
    free(buffer->data);
    memset(buffer, 0, sizeof(*buffer));
    free(buffer);
}

static LWCGLBuffer *buffer_clear(LWCGLBuffer *b) {
    if (!b) return NULL;
    b->position = 0;
    b->limit = b->capacity;
    b->mark = SIZE_MAX;
    return b;
}

static LWCGLBuffer *buffer_flip(LWCGLBuffer *b) {
    if (!b) return NULL;
    b->limit = b->position;
    b->position = 0;
    b->mark = SIZE_MAX;
    return b;
}

static LWCGLBuffer *buffer_rewind(LWCGLBuffer *b) {
    if (!b) return NULL;
    b->position = 0;
    b->mark = SIZE_MAX;
    return b;
}

static LWCGLBuffer *buffer_set_limit(LWCGLBuffer *b, size_t limit) {
    if (!b || limit > b->capacity) {
        set_error("invalid buffer limit");
        return NULL;
    }
    b->limit = limit;
    if (b->position > limit) b->position = limit;
    if (b->mark > limit) b->mark = SIZE_MAX;
    return b;
}

static LWCGLBuffer *buffer_set_position(LWCGLBuffer *b, size_t position) {
    if (!b || position > b->limit) {
        set_error("invalid buffer position");
        return NULL;
    }
    b->position = position;
    if (b->mark > position) b->mark = SIZE_MAX;
    return b;
}

static size_t buffer_remaining(const LWCGLBuffer *b) {
    return b && b->limit >= b->position ? b->limit - b->position : 0;
}

static LWCGLbool buffer_has_remaining(const LWCGLBuffer *b) {
    return buffer_remaining(b) != 0;
}

static void *buffer_address(LWCGLBuffer *b) {
    if (!b || !b->data || b->position > b->limit) return NULL;
    return (unsigned char *)b->data + b->position * b->elementSize;
}

static const void *buffer_const_address(const LWCGLBuffer *b) {
    if (!b || !b->data || b->position > b->limit) return NULL;
    return (const unsigned char *)b->data + b->position * b->elementSize;
}

static int buffer_put_raw(LWCGLBuffer *b, const void *value, size_t expected,
                          LWCGLBufferScalarType scalar_type) {
    if (!b || !value || b->elementSize != expected || b->scalarType != scalar_type ||
        b->position >= b->limit) {
        set_error("buffer overflow or type mismatch");
        return -1;
    }
    memcpy((unsigned char *)b->data + b->position * expected, value, expected);
    b->position++;
    return 0;
}

static int buffer_put_raw_at(LWCGLBuffer *b, size_t index, const void *value,
                             size_t expected, LWCGLBufferScalarType scalar_type) {
    if (!b || !value || b->elementSize != expected || b->scalarType != scalar_type ||
        index >= b->limit) {
        set_error("buffer index overflow or type mismatch");
        return -1;
    }
    memcpy((unsigned char *)b->data + index * expected, value, expected);
    return 0;
}

static int buffer_get_raw(LWCGLBuffer *b, void *value, size_t expected,
                          LWCGLBufferScalarType scalar_type) {
    if (!b || !value || b->elementSize != expected || b->scalarType != scalar_type ||
        b->position >= b->limit) {
        set_error("buffer underflow or type mismatch");
        return -1;
    }
    memcpy(value, (unsigned char *)b->data + b->position * expected, expected);
    b->position++;
    return 0;
}

static int buffer_get_raw_at(const LWCGLBuffer *b, size_t index, void *value,
                             size_t expected, LWCGLBufferScalarType scalar_type) {
    if (!b || !value || b->elementSize != expected || b->scalarType != scalar_type ||
        index >= b->limit) {
        set_error("buffer index overflow or type mismatch");
        return -1;
    }
    memcpy(value, (const unsigned char *)b->data + index * expected, expected);
    return 0;
}

static int buffer_put_byte(ByteBuffer *b, int8_t v) {
    return buffer_put_raw(b, &v, sizeof(v), LWCGL_BUFFER_BYTE);
}
static int buffer_put_int(IntBuffer *b, int32_t v) {
    return buffer_put_raw(b, &v, sizeof(v), LWCGL_BUFFER_INT);
}
static int buffer_put_int_at(IntBuffer *b, size_t i, int32_t v) {
    return buffer_put_raw_at(b, i, &v, sizeof(v), LWCGL_BUFFER_INT);
}
static int32_t buffer_get_int(IntBuffer *b) {
    int32_t v = 0;
    buffer_get_raw(b, &v, sizeof(v), LWCGL_BUFFER_INT);
    return v;
}
static int32_t buffer_get_int_at(const IntBuffer *b, size_t i) {
    int32_t v = 0;
    buffer_get_raw_at(b, i, &v, sizeof(v), LWCGL_BUFFER_INT);
    return v;
}
static int buffer_put_float(FloatBuffer *b, float v) {
    return buffer_put_raw(b, &v, sizeof(v), LWCGL_BUFFER_FLOAT);
}
static int buffer_put_float_at(FloatBuffer *b, size_t i, float v) {
    return buffer_put_raw_at(b, i, &v, sizeof(v), LWCGL_BUFFER_FLOAT);
}
static float buffer_get_float(FloatBuffer *b) {
    float v = 0.0f;
    buffer_get_raw(b, &v, sizeof(v), LWCGL_BUFFER_FLOAT);
    return v;
}
static float buffer_get_float_at(const FloatBuffer *b, size_t i) {
    float v = 0.0f;
    buffer_get_raw_at(b, i, &v, sizeof(v), LWCGL_BUFFER_FLOAT);
    return v;
}

BufferUtilsAPI BufferUtils = {
    buffer_create_byte,
    buffer_create_short,
    buffer_create_int,
    buffer_create_long,
    buffer_create_float,
    buffer_create_double,
    buffer_destroy
};

BufferAPI Buffer = {
    buffer_clear,
    buffer_flip,
    buffer_rewind,
    buffer_set_limit,
    buffer_set_position,
    buffer_remaining,
    buffer_has_remaining,
    buffer_address,
    buffer_const_address,
    buffer_put_byte,
    buffer_put_int,
    buffer_put_int_at,
    buffer_get_int,
    buffer_get_int_at,
    buffer_put_float,
    buffer_put_float_at,
    buffer_get_float,
    buffer_get_float_at
};

/* ------------------------------------------------------------------------- */
/* Display                                                                    */
/* ------------------------------------------------------------------------- */

static int display_set_mode(const DisplayMode *mode) {
    if (!mode || mode->width <= 0 || mode->height <= 0) {
        set_error("invalid DisplayMode");
        return -1;
    }
    g_display_mode = *mode;
    if (g_window && !g_fullscreen)
        glfwSetWindowSize(g_window, mode->width, mode->height);
    return 0;
}

static DisplayMode display_get_mode(void) { return g_display_mode; }

static DisplayMode display_get_desktop_mode(void) {
    DisplayMode out = g_display_mode;
    if (!glfwInit()) return out;
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = monitor ? glfwGetVideoMode(monitor) : NULL;
    if (mode) {
        out.width = mode->width;
        out.height = mode->height;
        out.bitsPerPixel = mode->redBits + mode->greenBits + mode->blueBits;
        out.frequency = mode->refreshRate;
        out.fullscreenCapable = LWCGL_TRUE;
    }
    if (!g_created) glfwTerminate();
    return out;
}

static int display_get_modes(DisplayMode **modes, size_t *count) {
    int n = 0;
    const GLFWvidmode *vmodes;
    GLFWmonitor *monitor;
    DisplayMode *result;

    if (!modes || !count) return -1;
    *modes = NULL;
    *count = 0;

    if (!glfwInit()) {
        set_error("glfwInit failed while enumerating display modes");
        return -1;
    }

    monitor = glfwGetPrimaryMonitor();
    vmodes = monitor ? glfwGetVideoModes(monitor, &n) : NULL;
    if (!vmodes || n <= 0) {
        if (!g_created) glfwTerminate();
        return 0;
    }

    result = (DisplayMode *)calloc((size_t)n, sizeof(*result));
    if (!result) {
        if (!g_created) glfwTerminate();
        set_error("out of memory enumerating display modes");
        return -1;
    }

    for (int i = 0; i < n; ++i) {
        result[i].width = vmodes[i].width;
        result[i].height = vmodes[i].height;
        result[i].bitsPerPixel = vmodes[i].redBits + vmodes[i].greenBits + vmodes[i].blueBits;
        result[i].frequency = vmodes[i].refreshRate;
        result[i].fullscreenCapable = LWCGL_TRUE;
    }

    *modes = result;
    *count = (size_t)n;
    if (!g_created) glfwTerminate();
    return 0;
}

static void display_free_modes(DisplayMode *modes) { free(modes); }

static void glfw_error_callback(int code, const char *message) {
    (void)code;
    set_error(message);
}

static int display_create(void) {
    if (g_created) return 0;
    lwcglClearError();
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        set_error("glfwInit failed");
        return -1;
    }

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_RESIZABLE, g_resizable ? GLFW_TRUE : GLFW_FALSE);

    GLFWmonitor *monitor = g_fullscreen ? glfwGetPrimaryMonitor() : NULL;
    g_window = glfwCreateWindow(g_display_mode.width, g_display_mode.height, g_title, monitor, NULL);
    if (!g_window) {
        set_error("failed to create OpenGL display");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(g_window);
    glfwSwapInterval(g_vsync ? 1 : 0);
    g_created = LWCGL_TRUE;
    return 0;
}

static void display_destroy(void) {
    if (g_window) glfwDestroyWindow(g_window);
    g_window = NULL;
    if (g_created) glfwTerminate();
    g_created = LWCGL_FALSE;
}

static LWCGLbool display_is_created(void) { return g_created; }

static void display_process_messages(void) {
    if (g_window) glfwPollEvents();
}

static void display_update(void) {
    if (!g_window) return;
    glfwSwapBuffers(g_window);
    glfwPollEvents();
}

static LWCGLbool display_close_requested(void) { return g_window && glfwWindowShouldClose(g_window); }
static LWCGLbool display_active(void) { return g_window && glfwGetWindowAttrib(g_window, GLFW_FOCUSED); }
static LWCGLbool display_visible(void) { return g_window && glfwGetWindowAttrib(g_window, GLFW_VISIBLE); }
static LWCGLbool display_dirty(void) { return LWCGL_TRUE; }
static int display_width(void) { int w = g_display_mode.width; if (g_window) glfwGetWindowSize(g_window, &w, NULL); return w; }
static int display_height(void) { int h = g_display_mode.height; if (g_window) glfwGetWindowSize(g_window, NULL, &h); return h; }
static int display_x(void) { int x = 0; if (g_window) glfwGetWindowPos(g_window, &x, NULL); return x; }
static int display_y(void) { int y = 0; if (g_window) glfwGetWindowPos(g_window, NULL, &y); return y; }

static void display_set_title(const char *title) {
    if (!title) title = "";
    snprintf(g_title, sizeof(g_title), "%s", title);
    if (g_window) glfwSetWindowTitle(g_window, g_title);
}

static void display_set_location(int x, int y) {
    g_windowed_x = x;
    g_windowed_y = y;
    if (g_window && !g_fullscreen) glfwSetWindowPos(g_window, x, y);
}

static void display_set_resizable(LWCGLbool value) {
    g_resizable = value ? LWCGL_TRUE : LWCGL_FALSE;
    if (g_window) glfwSetWindowAttrib(g_window, GLFW_RESIZABLE, g_resizable ? GLFW_TRUE : GLFW_FALSE);
}

static LWCGLbool display_is_resizable(void) { return g_resizable; }

static void display_set_vsync(LWCGLbool enabled) {
    g_vsync = enabled ? LWCGL_TRUE : LWCGL_FALSE;
    if (g_window) {
        glfwMakeContextCurrent(g_window);
        glfwSwapInterval(g_vsync ? 1 : 0);
    }
}

static int display_set_fullscreen(LWCGLbool fullscreen) {
    fullscreen = fullscreen ? LWCGL_TRUE : LWCGL_FALSE;
    if (fullscreen == g_fullscreen) return 0;
    g_fullscreen = fullscreen;
    if (!g_window) return 0;

    if (fullscreen) {
        glfwGetWindowPos(g_window, &g_windowed_x, &g_windowed_y);
        glfwGetWindowSize(g_window, &g_windowed_w, &g_windowed_h);
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = monitor ? glfwGetVideoMode(monitor) : NULL;
        if (!monitor || !mode) {
            set_error("unable to resolve primary monitor for fullscreen");
            return -1;
        }
        glfwSetWindowMonitor(g_window, monitor, 0, 0,
                             g_display_mode.width, g_display_mode.height,
                             g_display_mode.frequency > 0 ? g_display_mode.frequency : mode->refreshRate);
    } else {
        glfwSetWindowMonitor(g_window, NULL, g_windowed_x, g_windowed_y,
                             g_windowed_w, g_windowed_h, GLFW_DONT_CARE);
    }
    return 0;
}

static LWCGLbool display_is_fullscreen(void) { return g_fullscreen; }

static void display_sync(int fps) {
    static uint64_t next_frame;
    if (fps <= 0) return;
    uint64_t interval = 1000000000ull / (uint64_t)fps;
    uint64_t now = nano_time();
    if (!next_frame || next_frame + interval * 4 < now) next_frame = now;
    next_frame += interval;
    while ((now = nano_time()) < next_frame) {
        uint64_t left = next_frame - now;
        if (left > 2000000ull) {
            struct timespec ts;
            ts.tv_sec = 0;
            ts.tv_nsec = (long)(left - 1000000ull);
            nanosleep(&ts, NULL);
        }
    }
}

static void *display_native_window(void) { return g_window; }

DisplayAPI Display = {
    display_set_mode,
    display_get_mode,
    display_get_desktop_mode,
    display_get_modes,
    display_free_modes,
    display_create,
    display_destroy,
    display_is_created,
    display_update,
    display_process_messages,
    display_close_requested,
    display_active,
    display_visible,
    display_dirty,
    display_width,
    display_height,
    display_x,
    display_y,
    display_set_title,
    display_set_location,
    display_set_resizable,
    display_is_resizable,
    display_set_vsync,
    display_set_fullscreen,
    display_is_fullscreen,
    display_sync,
    display_native_window
};

/* ------------------------------------------------------------------------- */
/* Keyboard                                                                   */
/* ------------------------------------------------------------------------- */

typedef struct KeyEvent {
    int key;
    uint32_t character;
    LWCGLbool state;
    LWCGLbool repeat;
    uint64_t nanos;
} KeyEvent;

static KeyEvent g_key_events[LWCGL_EVENT_CAPACITY];
static size_t g_key_read;
static size_t g_key_write;
static KeyEvent g_key_current;
static LWCGLbool g_keyboard_created;
static LWCGLbool g_repeat_events;

static int lwjgl_to_glfw_key(int k) {
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

static int glfw_to_lwjgl_key(int k) {
    for (int i = 1; i < 256; ++i)
        if (lwjgl_to_glfw_key(i) == k) return i;
    return 0;
}

static void key_push(KeyEvent event) {
    size_t next = (g_key_write + 1) % LWCGL_EVENT_CAPACITY;
    if (next == g_key_read) g_key_read = (g_key_read + 1) % LWCGL_EVENT_CAPACITY;
    g_key_events[g_key_write] = event;
    g_key_write = next;
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    (void)window; (void)scancode; (void)mods;
    if (!g_keyboard_created) return;
    if (action == GLFW_REPEAT && !g_repeat_events) return;
    KeyEvent event;
    memset(&event, 0, sizeof(event));
    event.key = glfw_to_lwjgl_key(key);
    event.state = action != GLFW_RELEASE;
    event.repeat = action == GLFW_REPEAT;
    event.nanos = nano_time();
    key_push(event);
}

static void char_callback(GLFWwindow *window, unsigned int codepoint) {
    (void)window;
    if (!g_keyboard_created) return;
    KeyEvent event;
    memset(&event, 0, sizeof(event));
    event.key = 0;
    event.character = codepoint;
    event.state = LWCGL_TRUE;
    event.nanos = nano_time();
    key_push(event);
}

static int keyboard_create(void) {
    if (!g_window) { set_error("Keyboard.create requires Display.create first"); return -1; }
    g_keyboard_created = LWCGL_TRUE;
    glfwSetKeyCallback(g_window, key_callback);
    glfwSetCharCallback(g_window, char_callback);
    return 0;
}

static void keyboard_destroy(void) {
    if (g_window) { glfwSetKeyCallback(g_window, NULL); glfwSetCharCallback(g_window, NULL); }
    g_keyboard_created = LWCGL_FALSE;
    g_key_read = g_key_write = 0;
}

static LWCGLbool keyboard_is_created(void) { return g_keyboard_created; }
static void keyboard_poll(void) { }

static LWCGLbool keyboard_is_down(int key) {
    int gkey = lwjgl_to_glfw_key(key);
    if (!g_window || gkey == GLFW_KEY_UNKNOWN) return LWCGL_FALSE;
    int state = glfwGetKey(g_window, gkey);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

static const char *key_name_for_code(int key) {
    static char one[2];
    int gkey = lwjgl_to_glfw_key(key);
    if (gkey == GLFW_KEY_UNKNOWN) return NULL;
    const char *name = glfwGetKeyName(gkey, 0);
    if (name && name[0] && !name[1]) { one[0] = (char)toupper((unsigned char)name[0]); one[1] = 0; return one; }
    switch (key) {
        case 1: return "ESCAPE"; case 14: return "BACK"; case 15: return "TAB"; case 28: return "RETURN";
        case 29: return "LCONTROL"; case 42: return "LSHIFT"; case 54: return "RSHIFT"; case 56: return "LMENU";
        case 57: return "SPACE"; case 200: return "UP"; case 203: return "LEFT"; case 205: return "RIGHT";
        case 208: return "DOWN"; case 210: return "INSERT"; case 211: return "DELETE";
        default: return name;
    }
}

static int ascii_case_equal(const char *a, const char *b) {
    if (!a || !b) return 0;
    while (*a && *b) {
        if (toupper((unsigned char)*a++) != toupper((unsigned char)*b++)) return 0;
    }
    return *a == *b;
}

static int keyboard_key_index(const char *name) {
    for (int i = 0; i < 256; ++i) {
        const char *candidate = key_name_for_code(i);
        if (candidate && ascii_case_equal(candidate, name)) return i;
    }
    return 0;
}

static int keyboard_key_count(void) { return 256; }

static int keyboard_event_count(void) {
    if (g_key_write >= g_key_read) return (int)(g_key_write - g_key_read);
    return (int)(LWCGL_EVENT_CAPACITY - g_key_read + g_key_write);
}

static LWCGLbool keyboard_next(void) {
    if (g_key_read == g_key_write) return LWCGL_FALSE;
    g_key_current = g_key_events[g_key_read];
    g_key_read = (g_key_read + 1) % LWCGL_EVENT_CAPACITY;
    return LWCGL_TRUE;
}

static int keyboard_event_key(void) { return g_key_current.key; }
static uint32_t keyboard_event_char(void) { return g_key_current.character; }
static LWCGLbool keyboard_event_state(void) { return g_key_current.state; }
static uint64_t keyboard_event_nanos(void) { return g_key_current.nanos; }
static LWCGLbool keyboard_event_repeat(void) { return g_key_current.repeat; }
static void keyboard_enable_repeat(LWCGLbool enabled) { g_repeat_events = enabled ? LWCGL_TRUE : LWCGL_FALSE; }
static LWCGLbool keyboard_repeat_enabled(void) { return g_repeat_events; }

KeyboardAPI Keyboard = {
    0, 0, 1,
    2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,
    30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,
    59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,
    87,88,100,101,102,112,121,123,125,141,144,145,146,147,148,149,150,151,156,157,179,181,183,184,197,
    199,200,201,203,205,207,208,209,210,211,218,219,220,219,220,221,222,223,
    keyboard_create,
    keyboard_destroy,
    keyboard_is_created,
    keyboard_poll,
    keyboard_is_down,
    key_name_for_code,
    keyboard_key_index,
    keyboard_key_count,
    keyboard_event_count,
    keyboard_next,
    keyboard_event_key,
    keyboard_event_char,
    keyboard_event_state,
    keyboard_event_nanos,
    keyboard_event_repeat,
    keyboard_enable_repeat,
    keyboard_repeat_enabled
};

/* ------------------------------------------------------------------------- */
/* Mouse                                                                      */
/* ------------------------------------------------------------------------- */

typedef struct MouseEvent {
    int button;
    LWCGLbool state;
    int dx, dy;
    int x, y;
    int wheel;
    uint64_t nanos;
} MouseEvent;

static MouseEvent g_mouse_events[LWCGL_EVENT_CAPACITY];
static size_t g_mouse_read;
static size_t g_mouse_write;
static MouseEvent g_mouse_current;
static LWCGLbool g_mouse_created;
static LWCGLbool g_mouse_grabbed;
static LWCGLbool g_mouse_clip = LWCGL_TRUE;
static LWCGLbool g_mouse_have_position;
static double g_mouse_x;
static double g_mouse_y;
static int g_mouse_dx;
static int g_mouse_dy;
static int g_mouse_wheel;
static unsigned char g_mouse_buttons[LWCGL_MOUSE_BUTTONS];

static void mouse_push(MouseEvent event) {
    size_t next = (g_mouse_write + 1) % LWCGL_EVENT_CAPACITY;
    if (next == g_mouse_read) g_mouse_read = (g_mouse_read + 1) % LWCGL_EVENT_CAPACITY;
    g_mouse_events[g_mouse_write] = event;
    g_mouse_write = next;
}

static int mouse_public_y(double y) {
    int h = Display.getHeight();
    int out = h - 1 - (int)floor(y);
    if (g_mouse_clip) {
        if (out < 0) out = 0;
        if (out >= h) out = h > 0 ? h - 1 : 0;
    }
    return out;
}

static int mouse_public_x(double x) {
    int w = Display.getWidth();
    int out = (int)floor(x);
    if (g_mouse_clip) {
        if (out < 0) out = 0;
        if (out >= w) out = w > 0 ? w - 1 : 0;
    }
    return out;
}

static void cursor_pos_callback(GLFWwindow *window, double x, double y) {
    (void)window;
    if (!g_mouse_created) return;
    int dx = 0, dy = 0;
    if (g_mouse_have_position) {
        dx = (int)llround(x - g_mouse_x);
        dy = (int)llround(g_mouse_y - y);
    }
    g_mouse_have_position = LWCGL_TRUE;
    g_mouse_x = x;
    g_mouse_y = y;
    g_mouse_dx += dx;
    g_mouse_dy += dy;

    MouseEvent event;
    memset(&event, 0, sizeof(event));
    event.button = -1;
    event.dx = dx;
    event.dy = dy;
    event.x = mouse_public_x(x);
    event.y = mouse_public_y(y);
    event.nanos = nano_time();
    mouse_push(event);
}

static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    (void)window; (void)mods;
    if (!g_mouse_created) return;
    if (button >= 0 && button < LWCGL_MOUSE_BUTTONS) g_mouse_buttons[button] = action != GLFW_RELEASE;
    MouseEvent event;
    memset(&event, 0, sizeof(event));
    event.button = button;
    event.state = action != GLFW_RELEASE;
    event.x = mouse_public_x(g_mouse_x);
    event.y = mouse_public_y(g_mouse_y);
    event.nanos = nano_time();
    mouse_push(event);
}

static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    (void)window; (void)xoffset;
    if (!g_mouse_created) return;
    int wheel = (int)llround(yoffset * 120.0);
    g_mouse_wheel += wheel;
    MouseEvent event;
    memset(&event, 0, sizeof(event));
    event.button = -1;
    event.wheel = wheel;
    event.x = mouse_public_x(g_mouse_x);
    event.y = mouse_public_y(g_mouse_y);
    event.nanos = nano_time();
    mouse_push(event);
}

static int mouse_create(void) {
    if (!g_window) { set_error("Mouse.create requires Display.create first"); return -1; }
    g_mouse_created = LWCGL_TRUE;
    g_mouse_have_position = LWCGL_FALSE;
    glfwSetCursorPosCallback(g_window, cursor_pos_callback);
    glfwSetMouseButtonCallback(g_window, mouse_button_callback);
    glfwSetScrollCallback(g_window, scroll_callback);
    glfwGetCursorPos(g_window, &g_mouse_x, &g_mouse_y);
    g_mouse_have_position = LWCGL_TRUE;
    return 0;
}

static void mouse_destroy(void) {
    if (g_window) {
        glfwSetCursorPosCallback(g_window, NULL);
        glfwSetMouseButtonCallback(g_window, NULL);
        glfwSetScrollCallback(g_window, NULL);
        glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    g_mouse_created = LWCGL_FALSE;
    g_mouse_grabbed = LWCGL_FALSE;
    g_mouse_read = g_mouse_write = 0;
    memset(g_mouse_buttons, 0, sizeof(g_mouse_buttons));
}

static LWCGLbool mouse_is_created(void) { return g_mouse_created; }
static void mouse_poll(void) { }
static LWCGLbool mouse_button_down(int b) { return b >= 0 && b < LWCGL_MOUSE_BUTTONS && g_mouse_buttons[b]; }

static const char *mouse_button_name(int button) {
    static char name[16];
    if (button < 0 || button >= LWCGL_MOUSE_BUTTONS) return NULL;
    snprintf(name, sizeof(name), "BUTTON%d", button);
    return name;
}

static int mouse_button_index(const char *name) {
    int value;
    if (!name) return -1;
    if (sscanf(name, "BUTTON%d", &value) == 1 && value >= 0 && value < LWCGL_MOUSE_BUTTONS) return value;
    return -1;
}

static int mouse_button_count(void) { return 8; }
static LWCGLbool mouse_has_wheel(void) { return LWCGL_TRUE; }

static LWCGLbool mouse_next(void) {
    if (g_mouse_read == g_mouse_write) return LWCGL_FALSE;
    g_mouse_current = g_mouse_events[g_mouse_read];
    g_mouse_read = (g_mouse_read + 1) % LWCGL_EVENT_CAPACITY;
    return LWCGL_TRUE;
}

static int mouse_event_button(void) { return g_mouse_current.button; }
static LWCGLbool mouse_event_button_state(void) { return g_mouse_current.state; }
static int mouse_event_dx(void) { return g_mouse_current.dx; }
static int mouse_event_dy(void) { return g_mouse_current.dy; }
static int mouse_event_x(void) { return g_mouse_current.x; }
static int mouse_event_y(void) { return g_mouse_current.y; }
static int mouse_event_wheel(void) { return g_mouse_current.wheel; }
static uint64_t mouse_event_nanos(void) { return g_mouse_current.nanos; }
static int mouse_x(void) { return mouse_public_x(g_mouse_x); }
static int mouse_y(void) { return mouse_public_y(g_mouse_y); }
static int mouse_dx(void) { int v = g_mouse_dx; g_mouse_dx = 0; return v; }
static int mouse_dy(void) { int v = g_mouse_dy; g_mouse_dy = 0; return v; }
static int mouse_wheel(void) { int v = g_mouse_wheel; g_mouse_wheel = 0; return v; }
static LWCGLbool mouse_is_grabbed(void) { return g_mouse_grabbed; }

static void mouse_set_grabbed(LWCGLbool grabbed) {
    g_mouse_grabbed = grabbed ? LWCGL_TRUE : LWCGL_FALSE;
    g_mouse_dx = g_mouse_dy = 0;
    g_mouse_have_position = LWCGL_FALSE;
    if (g_window) glfwSetInputMode(g_window, GLFW_CURSOR, g_mouse_grabbed ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

static void mouse_set_position(int x, int y) {
    if (!g_window) return;
    int h = Display.getHeight();
    glfwSetCursorPos(g_window, (double)x, (double)(h - 1 - y));
}

static void mouse_set_clip(LWCGLbool clip) { g_mouse_clip = clip ? LWCGL_TRUE : LWCGL_FALSE; }
static LWCGLbool mouse_is_clip(void) { return g_mouse_clip; }

MouseAPI Mouse = {
    22,
    mouse_create,
    mouse_destroy,
    mouse_is_created,
    mouse_poll,
    mouse_button_down,
    mouse_button_name,
    mouse_button_index,
    mouse_button_count,
    mouse_has_wheel,
    mouse_next,
    mouse_event_button,
    mouse_event_button_state,
    mouse_event_dx,
    mouse_event_dy,
    mouse_event_x,
    mouse_event_y,
    mouse_event_wheel,
    mouse_event_nanos,
    mouse_x,
    mouse_y,
    mouse_dx,
    mouse_dy,
    mouse_wheel,
    mouse_is_grabbed,
    mouse_set_grabbed,
    mouse_set_position,
    mouse_set_clip,
    mouse_is_clip
};

/* ------------------------------------------------------------------------- */
/* Sys                                                                        */
/* ------------------------------------------------------------------------- */

static int sys_major(void) { return 2; }
static int sys_minor(void) { return 9; }
static int sys_patch(void) { return 3; }
static const char *sys_version(void) { return "2.9.3-lwcgl"; }
static uint64_t sys_time(void) { return nano_time() / 1000000ull; }
static uint64_t sys_resolution(void) { return 1000ull; }

static void sys_alert(const char *title, const char *message) {
    fprintf(stderr, "%s%s%s\n", title ? title : "LWJGL", title ? ": " : "", message ? message : "");
}

SysAPI Sys = {
    sys_major,
    sys_minor,
    sys_patch,
    sys_version,
    sys_time,
    sys_resolution,
    sys_alert
};
