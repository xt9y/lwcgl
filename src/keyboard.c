#define LWCGL_IMPLEMENTATION
#include <lwcgl/lwcgl.h>
#include "internal.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <ctype.h>
#include <string.h>

#define EVENT_CAPACITY 1024u

typedef struct KeyDef { int lwjgl; int glfw; const char *name; } KeyDef;
#define K(l,g,n) {l,g,n},
static const KeyDef keys[] = {
#include "key_table.inc"
};
#undef K

typedef struct KeyEvent { int key; uint32_t character; LWCGLbool state, repeat; uint64_t nanos; } KeyEvent;
static KeyEvent events[EVENT_CAPACITY], current;
static size_t read_pos, write_pos, last_char_slot;
static LWCGLbool created, repeat_enabled, last_char_valid, suppress_char;
static unsigned char down[256];
static uint64_t dropped;
static GLFWkeyfun chained_key;
static GLFWcharfun chained_char;

static int glfw_to_lwjgl(int key) {
    for (size_t i=0;i<sizeof keys/sizeof keys[0];i++) if (keys[i].glfw==key && key!=GLFW_KEY_UNKNOWN) return keys[i].lwjgl;
    return 0;
}
static int ascii_equal(const char*a,const char*b){if(!a||!b)return 0;while(*a&&*b){if(toupper((unsigned char)*a++)!=toupper((unsigned char)*b++))return 0;}return *a==*b;}
static void push(KeyEvent e){size_t slot=write_pos,next=(slot+1u)%EVENT_CAPACITY;if(next==read_pos){read_pos=(read_pos+1u)%EVENT_CAPACITY;dropped++;}events[slot]=e;write_pos=next;last_char_slot=slot;last_char_valid=e.state;}
static void key_callback(GLFWwindow*w,int key,int scancode,int action,int mods){if(!created)return;suppress_char=LWCGL_FALSE;int lwjgl=glfw_to_lwjgl(key);if(lwjgl>0&&lwjgl<256)down[lwjgl]=(unsigned char)(action!=GLFW_RELEASE);if(action==GLFW_REPEAT&&!repeat_enabled){suppress_char=LWCGL_TRUE;last_char_valid=LWCGL_FALSE;if(chained_key)chained_key(w,key,scancode,action,mods);return;}push((KeyEvent){lwjgl,0,action!=GLFW_RELEASE,action==GLFW_REPEAT,lwcglNanoTime()});if(action==GLFW_RELEASE)last_char_valid=LWCGL_FALSE;if(chained_key)chained_key(w,key,scancode,action,mods);}
static void char_callback(GLFWwindow*w,unsigned int codepoint){if(!created)return;if(suppress_char){suppress_char=LWCGL_FALSE;if(chained_char)chained_char(w,codepoint);return;}if(last_char_valid){events[last_char_slot].character=codepoint;last_char_valid=LWCGL_FALSE;}else{push((KeyEvent){0,codepoint,LWCGL_TRUE,LWCGL_FALSE,lwcglNanoTime()});last_char_valid=LWCGL_FALSE;}if(chained_char)chained_char(w,codepoint);}
static int create_keyboard(void){GLFWwindow*w=(GLFWwindow*)Display.getNativeWindow();if(!w){lwcglSetErrorInternal("Keyboard.create requires Display.create first");return -1;}if(created)return 0;memset(down,0,sizeof down);read_pos=write_pos=0;last_char_valid=suppress_char=LWCGL_FALSE;created=LWCGL_TRUE;chained_key=glfwSetKeyCallback(w,key_callback);chained_char=glfwSetCharCallback(w,char_callback);for(size_t i=0;i<sizeof keys/sizeof keys[0];i++){int l=keys[i].lwjgl,g=keys[i].glfw;if(g!=GLFW_KEY_UNKNOWN&&l>0&&l<256){int s=glfwGetKey(w,g);down[l]=(unsigned char)(s==GLFW_PRESS||s==GLFW_REPEAT);}}return 0;}
void lwcglKeyboardForceDestroy(void){GLFWwindow*w=(GLFWwindow*)Display.getNativeWindow();if(created&&w){glfwSetKeyCallback(w,chained_key);glfwSetCharCallback(w,chained_char);}chained_key=NULL;chained_char=NULL;created=LWCGL_FALSE;read_pos=write_pos=0;last_char_valid=LWCGL_FALSE;memset(down,0,sizeof down);}
static void destroy_keyboard(void){lwcglKeyboardForceDestroy();}
static LWCGLbool is_created(void){return created;}
static void poll(void){if(Display.getNativeWindow())glfwPollEvents();}
static LWCGLbool is_down(int key){return key>=0&&key<256&&down[key]?LWCGL_TRUE:LWCGL_FALSE;}
static const char *name_for(int key){if(key==0)return "NONE";for(size_t i=0;i<sizeof keys/sizeof keys[0];i++)if(keys[i].lwjgl==key)return keys[i].name;return NULL;}
static int index_for(const char*name){if(ascii_equal(name,"NONE"))return 0;for(size_t i=0;i<sizeof keys/sizeof keys[0];i++)if(ascii_equal(keys[i].name,name))return keys[i].lwjgl;return 0;}
static int key_count(void){return 256;}
static int event_count(void){return write_pos>=read_pos?(int)(write_pos-read_pos):(int)(EVENT_CAPACITY-read_pos+write_pos);}
static LWCGLbool next(void){if(read_pos==write_pos)return LWCGL_FALSE;current=events[read_pos];read_pos=(read_pos+1u)%EVENT_CAPACITY;return LWCGL_TRUE;}
static int event_key(void){return current.key;}static uint32_t event_char(void){return current.character;}static LWCGLbool event_state(void){return current.state;}static uint64_t event_nanos(void){return current.nanos;}static LWCGLbool event_repeat(void){return current.repeat;}static void enable_repeat(LWCGLbool v){repeat_enabled=v?LWCGL_TRUE:LWCGL_FALSE;}static LWCGLbool repeats_enabled(void){return repeat_enabled;}
uint64_t lwcglKeyboardDroppedEvents(void){return dropped;}

const KeyboardAPI Keyboard={
.CHAR_NONE=0,.KEY_NONE=0,.KEY_ESCAPE=1,.KEY_1=2,.KEY_2=3,.KEY_3=4,.KEY_4=5,.KEY_5=6,.KEY_6=7,.KEY_7=8,.KEY_8=9,.KEY_9=10,.KEY_0=11,.KEY_MINUS=12,.KEY_EQUALS=13,.KEY_BACK=14,.KEY_TAB=15,.KEY_Q=16,.KEY_W=17,.KEY_E=18,.KEY_R=19,.KEY_T=20,.KEY_Y=21,.KEY_U=22,.KEY_I=23,.KEY_O=24,.KEY_P=25,.KEY_LBRACKET=26,.KEY_RBRACKET=27,.KEY_RETURN=28,.KEY_LCONTROL=29,.KEY_A=30,.KEY_S=31,.KEY_D=32,.KEY_F=33,.KEY_G=34,.KEY_H=35,.KEY_J=36,.KEY_K=37,.KEY_L=38,.KEY_SEMICOLON=39,.KEY_APOSTROPHE=40,.KEY_GRAVE=41,.KEY_LSHIFT=42,.KEY_BACKSLASH=43,.KEY_Z=44,.KEY_X=45,.KEY_C=46,.KEY_V=47,.KEY_B=48,.KEY_N=49,.KEY_M=50,.KEY_COMMA=51,.KEY_PERIOD=52,.KEY_SLASH=53,.KEY_RSHIFT=54,.KEY_MULTIPLY=55,.KEY_LMENU=56,.KEY_SPACE=57,.KEY_CAPITAL=58,.KEY_F1=59,.KEY_F2=60,.KEY_F3=61,.KEY_F4=62,.KEY_F5=63,.KEY_F6=64,.KEY_F7=65,.KEY_F8=66,.KEY_F9=67,.KEY_F10=68,.KEY_NUMLOCK=69,.KEY_SCROLL=70,.KEY_NUMPAD7=71,.KEY_NUMPAD8=72,.KEY_NUMPAD9=73,.KEY_SUBTRACT=74,.KEY_NUMPAD4=75,.KEY_NUMPAD5=76,.KEY_NUMPAD6=77,.KEY_ADD=78,.KEY_NUMPAD1=79,.KEY_NUMPAD2=80,.KEY_NUMPAD3=81,.KEY_NUMPAD0=82,.KEY_DECIMAL=83,.KEY_F11=87,.KEY_F12=88,.KEY_F13=100,.KEY_F14=101,.KEY_F15=102,.KEY_KANA=112,.KEY_CONVERT=121,.KEY_NOCONVERT=123,.KEY_YEN=125,.KEY_NUMPADEQUALS=141,.KEY_CIRCUMFLEX=144,.KEY_AT=145,.KEY_COLON=146,.KEY_UNDERLINE=147,.KEY_KANJI=148,.KEY_STOP=149,.KEY_AX=150,.KEY_UNLABELED=151,.KEY_NUMPADENTER=156,.KEY_RCONTROL=157,.KEY_NUMPADCOMMA=179,.KEY_DIVIDE=181,.KEY_SYSRQ=183,.KEY_RMENU=184,.KEY_PAUSE=197,.KEY_HOME=199,.KEY_UP=200,.KEY_PRIOR=201,.KEY_LEFT=203,.KEY_RIGHT=205,.KEY_END=207,.KEY_DOWN=208,.KEY_NEXT=209,.KEY_INSERT=210,.KEY_DELETE=211,.KEY_CLEAR=218,.KEY_LMETA=219,.KEY_RMETA=220,.KEY_LWIN=219,.KEY_RWIN=220,.KEY_APPS=221,.KEY_POWER=222,.KEY_SLEEP=223,
.create=create_keyboard,.destroy=destroy_keyboard,.isCreated=is_created,.poll=poll,.isKeyDown=is_down,.getKeyName=name_for,.getKeyIndex=index_for,.getKeyCount=key_count,.getNumKeyboardEvents=event_count,.next=next,.getEventKey=event_key,.getEventCharacter=event_char,.getEventKeyState=event_state,.getEventNanoseconds=event_nanos,.isRepeatEvent=event_repeat,.enableRepeatEvents=enable_repeat,.areRepeatEventsEnabled=repeats_enabled,.abiSize=sizeof(KeyboardAPI),.abiVersion=LWCGL_ABI_VERSION};
