#define LWCGL_IMPLEMENTATION
#include <lwcgl/lwcgl.h>
#include <lwcgl/context.h>
#include <lwcgl/glmodern.h>
#include "internal.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static GLFWwindow *window;
static DisplayMode mode={640,480,0,0,LWCGL_FALSE};
static char *title;
static LWCGLbool created,resizable=LWCGL_TRUE,vsync,fullscreen,dirty=LWCGL_TRUE;
static int windowed_x=100,windowed_y=100,windowed_w=640,windowed_h=480,fb_width=640,fb_height=480;
static uint64_t sync_deadline; static int sync_fps;

static char *copy_string(const char*s){if(!s)s="";size_t n=strlen(s);char*out=(char*)malloc(n+1u);if(!out){lwcglSetErrorInternal("out of memory allocating title");return NULL;}memcpy(out,s,n+1u);return out;}
static void fb_callback(GLFWwindow*w,int width,int height){(void)w;fb_width=width;fb_height=height;dirty=LWCGL_TRUE;}
static void refresh_callback(GLFWwindow*w){(void)w;dirty=LWCGL_TRUE;}

static void clear_glfw_error(void) {
    (void)glfwGetError(NULL);
}

static int check_glfw_error(const char *operation) {
    const char *description = NULL;
    const int code = glfwGetError(&description);
    if (code == GLFW_NO_ERROR) return 0;
    char message[512];
    snprintf(message, sizeof message, "%s failed (GLFW %d)%s%s", operation, code,
             description ? ": " : "", description ? description : "");
    lwcglSetErrorInternal(message);
    return -1;
}
static int set_mode(const DisplayMode*m){
    if(!m||m->width<=0||m->height<=0){lwcglSetErrorInternal("invalid DisplayMode");return -1;}
    if(window&&fullscreen){
        GLFWmonitor*mon=glfwGetWindowMonitor(window);
        const GLFWvidmode*vm=mon?glfwGetVideoMode(mon):NULL;
        if(!mon||!vm){lwcglSetErrorInternal("fullscreen monitor unavailable");return -1;}
        const DisplayMode old=mode;
        clear_glfw_error();
        glfwSetWindowMonitor(window,mon,0,0,m->width,m->height,m->frequency>0?m->frequency:vm->refreshRate);
        if(check_glfw_error("Display.setDisplayMode")!=0){
            clear_glfw_error();
            glfwSetWindowMonitor(window,mon,0,0,old.width,old.height,old.frequency>0?old.frequency:vm->refreshRate);
            (void)glfwGetError(NULL);
            return -1;
        }
    } else if(window) {
        clear_glfw_error();
        glfwSetWindowSize(window,m->width,m->height);
        if(check_glfw_error("Display.setDisplayMode")!=0) return -1;
    }
    mode=*m;return 0;
}
static DisplayMode get_mode(void){return mode;}
static DisplayMode desktop_mode(void){DisplayMode out=mode;if(lwcglEnsureGlfw()!=0)return out;GLFWmonitor*mon=glfwGetPrimaryMonitor();const GLFWvidmode*vm=mon?glfwGetVideoMode(mon):NULL;if(vm){out.width=vm->width;out.height=vm->height;out.bitsPerPixel=vm->redBits+vm->greenBits+vm->blueBits;out.frequency=vm->refreshRate;out.fullscreenCapable=LWCGL_TRUE;}return out;}
static int available_modes(DisplayMode**out,size_t*count){
    if(!out||!count){lwcglSetErrorInternal("invalid display mode output");return -1;}*out=NULL;*count=0;if(lwcglEnsureGlfw()!=0)return -1;
    GLFWmonitor*mon=glfwGetPrimaryMonitor();int n=0;const GLFWvidmode*vm=mon?glfwGetVideoModes(mon,&n):NULL;if(!vm||n<=0)return 0;
    DisplayMode*result=(DisplayMode*)calloc((size_t)n,sizeof*result);if(!result){lwcglSetErrorInternal("out of memory enumerating display modes");return -1;}
    for(int i=0;i<n;i++)result[i]=(DisplayMode){vm[i].width,vm[i].height,vm[i].redBits+vm[i].greenBits+vm[i].blueBits,vm[i].refreshRate,LWCGL_TRUE};
    *out=result;*count=(size_t)n;return 0;
}
static void free_modes(DisplayMode*m){free(m);}
static int create_display(void){
    if (created) return 0;
    lwcglClearError();
    if (lwcglEnsureGlfw() != 0) return -1;
    glfwDefaultWindowHints();glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,lwcglRequestedContextMajorVersion());glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,lwcglRequestedContextMinorVersion());glfwWindowHint(GLFW_RESIZABLE,resizable?GLFW_TRUE:GLFW_FALSE);glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,lwcglRequestedContextDebug()?GLFW_TRUE:GLFW_FALSE);
    if(lwcglRequestedContextMajorVersion()>3||(lwcglRequestedContextMajorVersion()==3&&lwcglRequestedContextMinorVersion()>=2)){int p=lwcglRequestedContextProfile();glfwWindowHint(GLFW_OPENGL_PROFILE,p==LWCGL_CONTEXT_CORE_PROFILE?GLFW_OPENGL_CORE_PROFILE:p==LWCGL_CONTEXT_COMPATIBILITY_PROFILE?GLFW_OPENGL_COMPAT_PROFILE:GLFW_OPENGL_ANY_PROFILE);}
    GLFWmonitor*mon=fullscreen?glfwGetPrimaryMonitor():NULL;GLFWwindow*w=glfwCreateWindow(mode.width,mode.height,title?title:"Game",mon,NULL);if(!w){lwcglSetErrorInternal("failed to create OpenGL display");return -1;}
    window=w;glfwMakeContextCurrent(window);glfwSetFramebufferSizeCallback(window,fb_callback);glfwSetWindowRefreshCallback(window,refresh_callback);glfwGetFramebufferSize(window,&fb_width,&fb_height);
    if(lwcglLoadModernGL()!=0){char message[256];const char*missing=lwcglModernGLMissingFunction();snprintf(message,sizeof message,"OpenGL context incomplete%s%s",missing?": ":"",missing?missing:"");glfwDestroyWindow(window);window=NULL;lwcglSetErrorInternal(message);return -1;}
    glfwSwapInterval(vsync?1:0);created=LWCGL_TRUE;dirty=LWCGL_TRUE;sync_deadline=0;return 0;
}
static void destroy_display(void){if(!window){created=LWCGL_FALSE;return;}lwcglKeyboardForceDestroy();lwcglMouseForceDestroy();glfwDestroyWindow(window);window=NULL;created=LWCGL_FALSE;fullscreen=LWCGL_FALSE;sync_deadline=0;dirty=LWCGL_TRUE;}
static LWCGLbool is_created(void){return created;}
static void process_messages(void){if(window){dirty=LWCGL_FALSE;glfwPollEvents();}}
static void update_no_messages(void){if(window)glfwSwapBuffers(window);}
static void update(void){if(window){glfwSwapBuffers(window);process_messages();}}
static LWCGLbool close_requested(void){return window&&glfwWindowShouldClose(window)?LWCGL_TRUE:LWCGL_FALSE;}
static LWCGLbool active(void){return window&&glfwGetWindowAttrib(window,GLFW_FOCUSED)?LWCGL_TRUE:LWCGL_FALSE;}
static LWCGLbool visible(void){return window&&glfwGetWindowAttrib(window,GLFW_VISIBLE)?LWCGL_TRUE:LWCGL_FALSE;}
static LWCGLbool is_dirty(void){return dirty;}
static int width(void){if(window)glfwGetFramebufferSize(window,&fb_width,NULL);return fb_width>0?fb_width:mode.width;}
static int height(void){if(window)glfwGetFramebufferSize(window,NULL,&fb_height);return fb_height>0?fb_height:mode.height;}
static int xpos(void){int v=windowed_x;if(window&&!fullscreen)glfwGetWindowPos(window,&v,NULL);return v;}
static int ypos(void){int v=windowed_y;if(window&&!fullscreen)glfwGetWindowPos(window,NULL,&v);return v;}
static void set_title(const char*s){char*n=copy_string(s);if(!n)return;free(title);title=n;if(window)glfwSetWindowTitle(window,title);}
static void set_location(int x,int y){windowed_x=x;windowed_y=y;if(window&&!fullscreen)glfwSetWindowPos(window,x,y);}
static void set_resizable(LWCGLbool v){resizable=v?LWCGL_TRUE:LWCGL_FALSE;if(window)glfwSetWindowAttrib(window,GLFW_RESIZABLE,resizable?GLFW_TRUE:GLFW_FALSE);}
static LWCGLbool get_resizable(void){return resizable;}
static void set_vsync(LWCGLbool v){vsync=v?LWCGL_TRUE:LWCGL_FALSE;if(window){GLFWwindow*previous=glfwGetCurrentContext();if(previous!=window)glfwMakeContextCurrent(window);glfwSwapInterval(vsync?1:0);if(previous!=window)glfwMakeContextCurrent(previous);}}
static int set_fullscreen(LWCGLbool want){
    want=want?LWCGL_TRUE:LWCGL_FALSE;if(want==fullscreen)return 0;if(!window){fullscreen=want;return 0;}
    if(want){
        GLFWmonitor*mon=glfwGetPrimaryMonitor();const GLFWvidmode*vm=mon?glfwGetVideoMode(mon):NULL;
        if(!mon||!vm){lwcglSetErrorInternal("unable to resolve primary monitor for fullscreen");return -1;}
        int old_x,old_y,old_w,old_h;
        glfwGetWindowPos(window,&old_x,&old_y);glfwGetWindowSize(window,&old_w,&old_h);
        clear_glfw_error();
        glfwSetWindowMonitor(window,mon,0,0,mode.width,mode.height,mode.frequency>0?mode.frequency:vm->refreshRate);
        if(check_glfw_error("Display.setFullscreen")!=0){
            clear_glfw_error();
            glfwSetWindowMonitor(window,NULL,old_x,old_y,old_w,old_h,GLFW_DONT_CARE);
            (void)glfwGetError(NULL);
            return -1;
        }
        windowed_x=old_x;windowed_y=old_y;windowed_w=old_w;windowed_h=old_h;fullscreen=LWCGL_TRUE;
    } else {
        GLFWmonitor*old_monitor=glfwGetWindowMonitor(window);
        const GLFWvidmode*vm=old_monitor?glfwGetVideoMode(old_monitor):NULL;
        clear_glfw_error();
        glfwSetWindowMonitor(window,NULL,windowed_x,windowed_y,windowed_w,windowed_h,GLFW_DONT_CARE);
        if(check_glfw_error("Display.setFullscreen")!=0){
            if(old_monitor&&vm){clear_glfw_error();glfwSetWindowMonitor(window,old_monitor,0,0,mode.width,mode.height,mode.frequency>0?mode.frequency:vm->refreshRate);(void)glfwGetError(NULL);}
            return -1;
        }
        fullscreen=LWCGL_FALSE;
    }
    return 0;
}
static LWCGLbool get_fullscreen(void){return fullscreen;}
static void sync_display(int fps){
    if(fps<=0){sync_deadline=0;sync_fps=0;return;}uint64_t interval=1000000000ull/(uint64_t)fps,now=lwcglNanoTime();if(sync_fps!=fps||!sync_deadline||now>sync_deadline+interval*4u)sync_deadline=now+interval;else sync_deadline+=interval;sync_fps=fps;
#if defined(CLOCK_MONOTONIC) && defined(TIMER_ABSTIME) && !defined(__APPLE__)
    struct timespec ts={(time_t)(sync_deadline/1000000000ull),(long)(sync_deadline%1000000000ull)};(void)clock_nanosleep(CLOCK_MONOTONIC,TIMER_ABSTIME,&ts,NULL);
#else
    while((now=lwcglNanoTime())<sync_deadline){uint64_t left=sync_deadline-now;struct timespec ts={(time_t)(left/1000000000ull),(long)(left%1000000000ull)};if(nanosleep(&ts,NULL)==0)break;}
#endif
}
static void*native_window(void){return window;}
const DisplayAPI Display={set_mode,get_mode,desktop_mode,available_modes,free_modes,create_display,destroy_display,is_created,update,process_messages,close_requested,active,visible,is_dirty,width,height,xpos,ypos,set_title,set_location,set_resizable,get_resizable,set_vsync,set_fullscreen,get_fullscreen,sync_display,native_window,update_no_messages,sizeof(DisplayAPI),LWCGL_ABI_VERSION};
void lwcglDisplayUpdateNoMessages(void){update_no_messages();}
