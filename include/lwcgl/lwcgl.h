#ifndef LWCGL_LWCGL_H
#define LWCGL_LWCGL_H

#include <stddef.h>
#include <stdint.h>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef int LWCGLbool;

enum {
    LWCGL_FALSE = 0,
    LWCGL_TRUE = 1,
    LWCGL_VERSION_MAJOR = 2,
    LWCGL_VERSION_MINOR = 9,
    LWCGL_VERSION_PATCH = 3
};

typedef struct DisplayMode {
    int width;
    int height;
    int bitsPerPixel;
    int frequency;
    LWCGLbool fullscreenCapable;
} DisplayMode;

#ifdef __cplusplus
#define DisplayMode(width_, height_) DisplayMode{(width_), (height_), 0, 0, LWCGL_FALSE}
#else
#define DisplayMode(width_, height_) ((DisplayMode){(width_), (height_), 0, 0, LWCGL_FALSE})
#endif

typedef struct LWCGLBuffer {
    void *data;
    size_t elementSize;
    size_t capacity;
    size_t position;
    size_t limit;
    size_t mark;
} LWCGLBuffer;

typedef LWCGLBuffer ByteBuffer;
typedef LWCGLBuffer ShortBuffer;
typedef LWCGLBuffer IntBuffer;
typedef LWCGLBuffer LongBuffer;
typedef LWCGLBuffer FloatBuffer;
typedef LWCGLBuffer DoubleBuffer;

typedef struct BufferUtilsAPI {
    ByteBuffer *(*createByteBuffer)(size_t size);
    ShortBuffer *(*createShortBuffer)(size_t size);
    IntBuffer *(*createIntBuffer)(size_t size);
    LongBuffer *(*createLongBuffer)(size_t size);
    FloatBuffer *(*createFloatBuffer)(size_t size);
    DoubleBuffer *(*createDoubleBuffer)(size_t size);
    void (*destroy)(LWCGLBuffer *buffer);
} BufferUtilsAPI;

typedef struct BufferAPI {
    LWCGLBuffer *(*clear)(LWCGLBuffer *buffer);
    LWCGLBuffer *(*flip)(LWCGLBuffer *buffer);
    LWCGLBuffer *(*rewind)(LWCGLBuffer *buffer);
    LWCGLBuffer *(*setLimit)(LWCGLBuffer *buffer, size_t limit);
    LWCGLBuffer *(*setPosition)(LWCGLBuffer *buffer, size_t position);
    size_t (*remaining)(const LWCGLBuffer *buffer);
    LWCGLbool (*hasRemaining)(const LWCGLBuffer *buffer);
    void *(*address)(LWCGLBuffer *buffer);
    const void *(*constAddress)(const LWCGLBuffer *buffer);
    int (*putByte)(ByteBuffer *buffer, int8_t value);
    int (*putInt)(IntBuffer *buffer, int32_t value);
    int (*putIntAt)(IntBuffer *buffer, size_t index, int32_t value);
    int32_t (*getInt)(IntBuffer *buffer);
    int32_t (*getIntAt)(const IntBuffer *buffer, size_t index);
    int (*putFloat)(FloatBuffer *buffer, float value);
    int (*putFloatAt)(FloatBuffer *buffer, size_t index, float value);
    float (*getFloat)(FloatBuffer *buffer);
    float (*getFloatAt)(const FloatBuffer *buffer, size_t index);
} BufferAPI;

typedef struct DisplayAPI {
    int (*setDisplayMode)(const DisplayMode *mode);
    DisplayMode (*getDisplayMode)(void);
    DisplayMode (*getDesktopDisplayMode)(void);
    int (*getAvailableDisplayModes)(DisplayMode **modes, size_t *count);
    void (*freeDisplayModes)(DisplayMode *modes);

    int (*create)(void);
    void (*destroy)(void);
    LWCGLbool (*isCreated)(void);

    void (*update)(void);
    void (*processMessages)(void);
    LWCGLbool (*isCloseRequested)(void);
    LWCGLbool (*isActive)(void);
    LWCGLbool (*isVisible)(void);
    LWCGLbool (*isDirty)(void);

    int (*getWidth)(void);
    int (*getHeight)(void);
    int (*getX)(void);
    int (*getY)(void);

    void (*setTitle)(const char *title);
    void (*setLocation)(int x, int y);
    void (*setResizable)(LWCGLbool resizable);
    LWCGLbool (*isResizable)(void);
    void (*setVSyncEnabled)(LWCGLbool enabled);

    int (*setFullscreen)(LWCGLbool fullscreen);
    LWCGLbool (*isFullscreen)(void);

    void (*sync)(int fps);
    void *(*getNativeWindow)(void);
} DisplayAPI;

typedef struct KeyboardAPI {
    int CHAR_NONE;
    int KEY_NONE;
    int KEY_ESCAPE;
    int KEY_1;
    int KEY_2;
    int KEY_3;
    int KEY_4;
    int KEY_5;
    int KEY_6;
    int KEY_7;
    int KEY_8;
    int KEY_9;
    int KEY_0;
    int KEY_MINUS;
    int KEY_EQUALS;
    int KEY_BACK;
    int KEY_TAB;
    int KEY_Q;
    int KEY_W;
    int KEY_E;
    int KEY_R;
    int KEY_T;
    int KEY_Y;
    int KEY_U;
    int KEY_I;
    int KEY_O;
    int KEY_P;
    int KEY_LBRACKET;
    int KEY_RBRACKET;
    int KEY_RETURN;
    int KEY_LCONTROL;
    int KEY_A;
    int KEY_S;
    int KEY_D;
    int KEY_F;
    int KEY_G;
    int KEY_H;
    int KEY_J;
    int KEY_K;
    int KEY_L;
    int KEY_SEMICOLON;
    int KEY_APOSTROPHE;
    int KEY_GRAVE;
    int KEY_LSHIFT;
    int KEY_BACKSLASH;
    int KEY_Z;
    int KEY_X;
    int KEY_C;
    int KEY_V;
    int KEY_B;
    int KEY_N;
    int KEY_M;
    int KEY_COMMA;
    int KEY_PERIOD;
    int KEY_SLASH;
    int KEY_RSHIFT;
    int KEY_MULTIPLY;
    int KEY_LMENU;
    int KEY_SPACE;
    int KEY_CAPITAL;
    int KEY_F1;
    int KEY_F2;
    int KEY_F3;
    int KEY_F4;
    int KEY_F5;
    int KEY_F6;
    int KEY_F7;
    int KEY_F8;
    int KEY_F9;
    int KEY_F10;
    int KEY_NUMLOCK;
    int KEY_SCROLL;
    int KEY_NUMPAD7;
    int KEY_NUMPAD8;
    int KEY_NUMPAD9;
    int KEY_SUBTRACT;
    int KEY_NUMPAD4;
    int KEY_NUMPAD5;
    int KEY_NUMPAD6;
    int KEY_ADD;
    int KEY_NUMPAD1;
    int KEY_NUMPAD2;
    int KEY_NUMPAD3;
    int KEY_NUMPAD0;
    int KEY_DECIMAL;
    int KEY_F11;
    int KEY_F12;
    int KEY_F13;
    int KEY_F14;
    int KEY_F15;
    int KEY_KANA;
    int KEY_CONVERT;
    int KEY_NOCONVERT;
    int KEY_YEN;
    int KEY_NUMPADEQUALS;
    int KEY_CIRCUMFLEX;
    int KEY_AT;
    int KEY_COLON;
    int KEY_UNDERLINE;
    int KEY_KANJI;
    int KEY_STOP;
    int KEY_AX;
    int KEY_UNLABELED;
    int KEY_NUMPADENTER;
    int KEY_RCONTROL;
    int KEY_NUMPADCOMMA;
    int KEY_DIVIDE;
    int KEY_SYSRQ;
    int KEY_RMENU;
    int KEY_PAUSE;
    int KEY_HOME;
    int KEY_UP;
    int KEY_PRIOR;
    int KEY_LEFT;
    int KEY_RIGHT;
    int KEY_END;
    int KEY_DOWN;
    int KEY_NEXT;
    int KEY_INSERT;
    int KEY_DELETE;
    int KEY_CLEAR;
    int KEY_LMETA;
    int KEY_RMETA;
    int KEY_LWIN;
    int KEY_RWIN;
    int KEY_APPS;
    int KEY_POWER;
    int KEY_SLEEP;

    int (*create)(void);
    void (*destroy)(void);
    LWCGLbool (*isCreated)(void);
    void (*poll)(void);
    LWCGLbool (*isKeyDown)(int key);
    const char *(*getKeyName)(int key);
    int (*getKeyIndex)(const char *name);
    int (*getKeyCount)(void);
    int (*getNumKeyboardEvents)(void);
    LWCGLbool (*next)(void);
    int (*getEventKey)(void);
    uint32_t (*getEventCharacter)(void);
    LWCGLbool (*getEventKeyState)(void);
    uint64_t (*getEventNanoseconds)(void);
    LWCGLbool (*isRepeatEvent)(void);
    void (*enableRepeatEvents)(LWCGLbool enabled);
    LWCGLbool (*areRepeatEventsEnabled)(void);
} KeyboardAPI;

typedef struct MouseAPI {
    int EVENT_SIZE;

    int (*create)(void);
    void (*destroy)(void);
    LWCGLbool (*isCreated)(void);
    void (*poll)(void);

    LWCGLbool (*isButtonDown)(int button);
    const char *(*getButtonName)(int button);
    int (*getButtonIndex)(const char *name);
    int (*getButtonCount)(void);
    LWCGLbool (*hasWheel)(void);

    LWCGLbool (*next)(void);
    int (*getEventButton)(void);
    LWCGLbool (*getEventButtonState)(void);
    int (*getEventDX)(void);
    int (*getEventDY)(void);
    int (*getEventX)(void);
    int (*getEventY)(void);
    int (*getEventDWheel)(void);
    uint64_t (*getEventNanoseconds)(void);

    int (*getX)(void);
    int (*getY)(void);
    int (*getDX)(void);
    int (*getDY)(void);
    int (*getDWheel)(void);

    LWCGLbool (*isGrabbed)(void);
    void (*setGrabbed)(LWCGLbool grabbed);
    void (*setCursorPosition)(int x, int y);
    void (*setClipMouseCoordinatesToWindow)(LWCGLbool clip);
    LWCGLbool (*isClipMouseCoordinatesToWindow)(void);
} MouseAPI;

typedef struct SysAPI {
    int (*getVersionMajor)(void);
    int (*getVersionMinor)(void);
    int (*getVersionPatch)(void);
    const char *(*getVersion)(void);
    uint64_t (*getTime)(void);
    uint64_t (*getTimerResolution)(void);
    void (*alert)(const char *title, const char *message);
} SysAPI;

extern BufferUtilsAPI BufferUtils;
extern BufferAPI Buffer;
extern DisplayAPI Display;
extern KeyboardAPI Keyboard;
extern MouseAPI Mouse;
extern SysAPI Sys;

const char *lwcglGetLastError(void);
void lwcglClearError(void);

#ifdef __cplusplus
}
#endif

#include <lwcgl/gl11_compat.h>

#endif
