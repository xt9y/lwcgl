#include <lwcgl/context.h>
static int major=2,minor=1,profile=LWCGL_CONTEXT_ANY_PROFILE,debug;
void lwcglSetContextVersion(int M,int m){if(M<1||m<0){M=2;m=1;}major=M;minor=m;}
void lwcglSetContextProfile(int p){profile=(p==LWCGL_CONTEXT_CORE_PROFILE||p==LWCGL_CONTEXT_COMPATIBILITY_PROFILE)?p:LWCGL_CONTEXT_ANY_PROFILE;}
void lwcglSetContextDebug(int v){debug=v?1:0;}
int lwcglRequestedContextMajorVersion(void){return major;}int lwcglRequestedContextMinorVersion(void){return minor;}int lwcglRequestedContextProfile(void){return profile;}int lwcglRequestedContextDebug(void){return debug;}
