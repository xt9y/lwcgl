#include <lwcgl/lwcgl.h>
#include "lwcgl_internal.h"
#include <stdio.h>
static LWCGL_THREAD_LOCAL char g_error[512];
static LWCGL_THREAD_LOCAL LWCGLResultCode g_error_code;
void lwcglSetErrorInternal(LWCGLResultCode code, const char *message){g_error_code=code;snprintf(g_error,sizeof g_error,"%s",message?message:"unknown error");}
const char *lwcglGetLastError(void){return g_error[0]?g_error:NULL;}
LWCGLResultCode lwcglGetLastErrorCode(void){return g_error_code;}
void lwcglClearError(void){g_error[0]='\0';g_error_code=LWCGL_OK;}
static int vmaj(void){return LWCGL_VERSION_MAJOR;} static int vmin(void){return LWCGL_VERSION_MINOR;} static int vpatch(void){return LWCGL_VERSION_PATCH;}
const char *lwcglGetVersion(void){return LWCGL_VERSION_STRING;}
LWCGLVersion lwcglGetVersionInfo(void){LWCGLVersion v={LWCGL_VERSION_MAJOR,LWCGL_VERSION_MINOR,LWCGL_VERSION_PATCH,LWCGL_VERSION_STRING};return v;}
const LWCGLAPI LWCGL={sizeof(LWCGLAPI),LWCGL_ABI_VERSION,vmaj,vmin,vpatch,lwcglGetVersion,lwcglGetVersionInfo};
