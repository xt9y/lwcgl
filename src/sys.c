#include <lwcgl/lwcgl.h>
#include "internal.h"
#include <stdio.h>
static int major(void){return 2;}static int minor(void){return 9;}static int patch(void){return 3;}static const char*version(void){return "2.9.3-lwcgl";}static uint64_t time_value(void){return lwcglNanoTime()/1000000ull;}static uint64_t resolution(void){return 1000ull;}static void alert(const char*t,const char*m){fprintf(stderr,"%s%s%s\n",t?t:"LWJGL",t?": ":"",m?m:"");}
const SysAPI Sys={major,minor,patch,version,time_value,resolution,alert,sizeof(SysAPI),LWCGL_ABI_VERSION};
