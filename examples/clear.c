#define LWCGL_ENABLE_LWJGL2_COMPAT 1
#include <lwcgl/lwcgl.h>
#include <lwcgl/context.h>
int main(void){
    DisplayMode mode={640,480,0,0,LWCGL_FALSE};
    if(Display.setDisplayMode(&mode)!=0) return 1;
    lwcglSetContextVersion(2,1);
    lwcglSetContextProfile(LWCGL_CONTEXT_COMPATIBILITY_PROFILE);
    if(Display.create()!=0) return 1;
    glViewport(0,0,Display.getWidth(),Display.getHeight());
    glClearColor(0.1f,0.1f,0.1f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    Display.updateNoMessages();
    Display.destroy();
    return 0;
}
