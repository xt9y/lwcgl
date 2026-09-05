#include <lwcgl/lwcgl.h>
#include <assert.h>
#include <string.h>
int main(void){assert(MemoryUtil.memLengthNT1("abc")==3);LWCGLMemoryStack*a=MemoryStack.stackPushCapacity(128);assert(a);char*s=MemoryStack.UTF8(a,"hello");assert(s&&strcmp(s,"hello")==0);void*p1=MemoryStack.malloc(a,16);assert(p1);LWCGLMemoryStack*b=MemoryStack.stackPushCapacity(64);assert(b);void*p2=MemoryStack.malloc(b,16);assert(p2);MemoryStack.stackPop(a);assert(lwcglGetLastErrorCode()==LWCGL_ERROR_STATE);MemoryStack.stackPop(b);MemoryStack.stackPop(a);LWCGLMemoryStack*c=MemoryStack.stackPushCapacity(128);assert(c);char*s2=MemoryStack.UTF8(c,"hello");assert(s2&&strcmp(s2,"hello")==0);void*p3=MemoryStack.malloc(c,16);assert(p3==p1);MemoryStack.stackPop(c);MemoryStack.trimThreadArena();return 0;}
