#include <lwcgl/lwcgl.h>
#include "../src/lwcgl_internal.h"
#include <assert.h>
#include <string.h>

void fake_set_context(int id);
void *fake_context_ptr(int id);
void fake_set_missing(const char *name);
void fake_clear_a(LWCGLbitfield mask);
void fake_clear_b(LWCGLbitfield mask);

int main(void) {
    fake_set_context(1);
    lwcglGLRegisterContext(fake_context_ptr(1));
    GLCapabilities *a = GL.createCapabilities();
    assert(a);
    assert(GL.getCapabilities() == a);
    assert(GL11.glClear == fake_clear_a);
    assert(a->OpenGL46 == LWCGL_TRUE);
    assert(GL.isFunctionAvailable("glClear") == LWCGL_TRUE);
    assert(GL.isExtensionSupported("GL_FAKE_extension") == LWCGL_TRUE);

    fake_set_missing("glDrawElements");
    GLCapabilities *failed = GL.createCapabilities();
    assert(!failed);
    assert(GL.getCapabilities() == a);
    assert(GL11.glClear == fake_clear_a);
    assert(lwcglGetLastErrorCode() == LWCGL_ERROR_UNSUPPORTED);
    fake_set_missing(0);

    fake_set_context(2);
    lwcglGLRegisterContext(fake_context_ptr(2));
    lwcglGLContextChanged(fake_context_ptr(2));
    assert(GL.getCapabilities() == 0);
    assert(GL11.glClear == 0);
    assert(GL.setCapabilities(a) == -1);
    assert(lwcglGetLastErrorCode() == LWCGL_ERROR_CONTEXT_MISMATCH);

    GLCapabilities *b = GL.createCapabilities();
    assert(b);
    assert(GL11.glClear == fake_clear_b);

    fake_set_context(1);
    lwcglGLContextChanged(fake_context_ptr(1));
    assert(GL11.glClear == 0);
    assert(GL.setCapabilities(a) == 0);
    assert(GL11.glClear == fake_clear_a);

    lwcglGLUnregisterContext(fake_context_ptr(1));
    assert(GL11.glClear == 0);
    assert(GL.setCapabilities(a) == -1);

    fake_set_context(2);
    lwcglGLContextChanged(fake_context_ptr(2));
    assert(GL.setCapabilities(b) == 0);
    GL.destroyCapabilities(b);
    GL.destroyCapabilities(a);
    return 0;
}
