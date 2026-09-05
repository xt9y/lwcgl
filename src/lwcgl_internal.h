#ifndef LWCGL_INTERNAL_H
#define LWCGL_INTERNAL_H
#include <lwcgl/lwcgl.h>
void lwcglSetErrorInternal(LWCGLResultCode code, const char *message);
void lwcglGLRegisterContext(void *context);
void lwcglGLUnregisterContext(void *context);
void lwcglGLInvalidateAllContexts(void);
void lwcglGLContextChanged(void *context);
uint64_t lwcglGLContextGeneration(void *context);
#endif
