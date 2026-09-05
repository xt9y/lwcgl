#include <lwcgl/lwcgl.h>
#include "lwcgl_internal.h"
#include <stdlib.h>
#include <string.h>
#define DEFAULT_CAP (64u*1024u)
#define ALIGNMENT 16u
#define MAX_DEPTH 64u
struct LWCGLMemoryStack { size_t base,pointer,limit,saved_parent; unsigned depth; };
typedef struct Arena { unsigned char *memory; size_t capacity,pointer; unsigned depth; LWCGLMemoryStack frames[MAX_DEPTH]; } Arena;
static LWCGL_THREAD_LOCAL Arena arena;
static int align_up(size_t v,size_t *out){size_t mask=ALIGNMENT-1u;if(v>SIZE_MAX-mask)return -1;*out=(v+mask)&~mask;return 0;}
static int ensure(size_t need){if(need<=arena.capacity)return 0;size_t cap=arena.capacity?arena.capacity:DEFAULT_CAP;while(cap<need){if(cap>SIZE_MAX/2u){cap=need;break;}cap*=2u;}unsigned char*n=(unsigned char*)realloc(arena.memory,cap);if(!n){lwcglSetErrorInternal(LWCGL_ERROR_OUT_OF_MEMORY,"MemoryStack: arena growth failed");return -1;}arena.memory=n;arena.capacity=cap;return 0;}
static void *m_alloc(size_t n){lwcglClearError();if(!n)n=1;void*p=malloc(n);if(!p)lwcglSetErrorInternal(LWCGL_ERROR_OUT_OF_MEMORY,"MemoryUtil.memAlloc: allocation failed");return p;}
static void *m_calloc(size_t c,size_t n){lwcglClearError();if(c&&n>SIZE_MAX/c){lwcglSetErrorInternal(LWCGL_ERROR_OVERFLOW,"MemoryUtil.memCalloc: size overflow");return NULL;}if(!c||!n){c=n=1;}void*p=calloc(c,n);if(!p)lwcglSetErrorInternal(LWCGL_ERROR_OUT_OF_MEMORY,"MemoryUtil.memCalloc: allocation failed");return p;}
static void *m_realloc(void*p,size_t n){lwcglClearError();if(!n)n=1;void*q=realloc(p,n);if(!q)lwcglSetErrorInternal(LWCGL_ERROR_OUT_OF_MEMORY,"MemoryUtil.memRealloc: allocation failed");return q;}
static void m_free(void*p){lwcglClearError();free(p);} static uintptr_t m_addr(const void*p){lwcglClearError();return(uintptr_t)p;} static void *m_ptr(uintptr_t a){lwcglClearError();return(void*)a;}
static char *m_str(const char*s){lwcglClearError();if(!s){lwcglSetErrorInternal(LWCGL_ERROR_INVALID_ARGUMENT,"MemoryUtil string: text is NULL");return NULL;}size_t n=strlen(s);char*p=(char*)malloc(n+1u);if(!p){lwcglSetErrorInternal(LWCGL_ERROR_OUT_OF_MEMORY,"MemoryUtil string: allocation failed");return NULL;}memcpy(p,s,n+1u);return p;}
static size_t m_len(const char*s){lwcglClearError();return s?strlen(s):0u;}
static LWCGLMemoryStack *push_cap(size_t cap){lwcglClearError();if(!cap)cap=DEFAULT_CAP;if(arena.depth>=MAX_DEPTH){lwcglSetErrorInternal(LWCGL_ERROR_STATE,"MemoryStack.stackPush: maximum nesting depth reached");return NULL;}size_t base;if(align_up(arena.pointer,&base)||cap>SIZE_MAX-base){lwcglSetErrorInternal(LWCGL_ERROR_OVERFLOW,"MemoryStack.stackPush: size overflow");return NULL;}size_t limit=base+cap;if(ensure(limit))return NULL;LWCGLMemoryStack*f=&arena.frames[arena.depth];f->base=base;f->pointer=base;f->limit=limit;f->saved_parent=arena.pointer;f->depth=arena.depth++;arena.pointer=base;return f;}
static LWCGLMemoryStack *push(void){return push_cap(DEFAULT_CAP);}
static void pop(LWCGLMemoryStack*f){lwcglClearError();if(!f)return;if(!arena.depth||f!=&arena.frames[arena.depth-1u]){lwcglSetErrorInternal(LWCGL_ERROR_STATE,"MemoryStack.stackPop: frames must be popped in LIFO order");return;}arena.pointer=f->saved_parent;memset(f,0,sizeof *f);arena.depth--;}
static void *s_malloc(LWCGLMemoryStack*f,size_t n){lwcglClearError();if(!f||!arena.depth||f!=&arena.frames[arena.depth-1u]){lwcglSetErrorInternal(LWCGL_ERROR_STATE,"MemoryStack.malloc: stack frame is not current");return NULL;}size_t a;if(align_up(f->pointer,&a)||a>f->limit||n>f->limit-a){lwcglSetErrorInternal(LWCGL_ERROR_OVERFLOW,"MemoryStack.malloc: stack overflow");return NULL;}void*p=arena.memory+a;f->pointer=a+n;arena.pointer=f->pointer;return p;}
static void *s_calloc(LWCGLMemoryStack*f,size_t c,size_t n){if(c&&n>SIZE_MAX/c){lwcglSetErrorInternal(LWCGL_ERROR_OVERFLOW,"MemoryStack.calloc: size overflow");return NULL;}size_t b=c*n;void*p=s_malloc(f,b);if(p&&b)memset(p,0,b);return p;}
static char *s_str(LWCGLMemoryStack*f,const char*s){if(!s){lwcglSetErrorInternal(LWCGL_ERROR_INVALID_ARGUMENT,"MemoryStack string: text is NULL");return NULL;}size_t n=strlen(s);char*p=(char*)s_malloc(f,n+1u);if(p)memcpy(p,s,n+1u);return p;}
static size_t s_pos(const LWCGLMemoryStack*f){return f?f->pointer-f->base:0u;} static size_t s_cap(const LWCGLMemoryStack*f){return f?f->limit-f->base:0u;}
static void trim(void){lwcglClearError();if(arena.depth){lwcglSetErrorInternal(LWCGL_ERROR_STATE,"MemoryStack.trimThreadArena: stack frames are still active");return;}free(arena.memory);memset(&arena,0,sizeof arena);}
const MemoryUtilAPI MemoryUtil={m_alloc,m_calloc,m_realloc,m_free,m_addr,m_ptr,m_str,m_str,m_len};
const MemoryStackAPI MemoryStack={push,push_cap,pop,s_malloc,s_calloc,s_str,s_str,s_pos,s_cap,trim};
