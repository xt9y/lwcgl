#define LWCGL_ENABLE_LWJGL2_COMPAT 1
#include <lwcgl/lwcgl.h>
#include <lwcgl/context.h>
#include <lwcgl/glmodern.h>
#include <stdio.h>
int main(void) {
    DisplayMode mode = {320, 200, 0, 0, LWCGL_FALSE};
    if (Display.setDisplayMode(&mode) != 0) return 1;
    Display.setTitle("lwcgl runtime smoke: titles are dynamically stored and are not truncated to the old fixed buffer");
    lwcglSetContextVersion(2, 1);
    lwcglSetContextProfile(LWCGL_CONTEXT_COMPATIBILITY_PROFILE);
    if (Display.create() != 0) { fprintf(stderr, "%s\n", lwcglGetLastError()); return 2; }
    if (!lwcglModernGLAvailable()) return 3;
    if (Display.getWidth() <= 0 || Display.getHeight() <= 0) return 4;

    (void)glGetInteger(GL_VIEWPORT);
    IntBuffer *viewport = BufferUtils.createIntBuffer(4);
    if (!viewport) return 5;
    glGetInteger(GL_VIEWPORT, viewport);
    BufferUtils.destroy(viewport);

    glViewport(0, 0, Display.getWidth(), Display.getHeight());
    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glFinish();
    unsigned char pixel[4] = {0, 0, 0, 0};
    glReadPixels(0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
    if (pixel[0] < 40 || pixel[1] < 90 || pixel[2] < 140 || pixel[3] < 200) return 6;

    if (Keyboard.create() != 0 || Mouse.create() != 0) return 7;
    Keyboard.poll(); Mouse.poll(); Display.processMessages();
    Mouse.setGrabbed(LWCGL_TRUE); Mouse.setGrabbed(LWCGL_FALSE);
    Display.updateNoMessages();
    Display.destroy();
    if (Keyboard.isCreated() || Mouse.isCreated() || Display.isCreated()) return 8;
    return 0;
}
