#ifndef LWCGL_INTERNAL_H
#define LWCGL_INTERNAL_H
#include <lwcgl/lwcgl.h>
void lwcglSetErrorInternal(const char *message);
uint64_t lwcglNanoTime(void);
int lwcglEnsureGlfw(void);
void lwcglKeyboardForceDestroy(void);
void lwcglMouseForceDestroy(void);
#endif
