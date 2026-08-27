#include <lwcgl/lwcgl.h>

static void compile_rubydung_surface() {
    DisplayMode *mode = new DisplayMode(1024, 768);
    Display.setDisplayMode(mode);
    Display.setTitle("rd-132328");
    Display.create();

    Keyboard.create();
    Mouse.create();
    Mouse.setGrabbed(true);

    FloatBuffer *fogColor = BufferUtils.createFloatBuffer(4);
    IntBuffer *viewportBuffer = BufferUtils.createIntBuffer(16);
    IntBuffer *selectBuffer = BufferUtils.createIntBuffer(2000);

    Buffer.putFloat(fogColor, 14.0f / 255.0f);
    Buffer.putFloat(fogColor, 11.0f / 255.0f);
    Buffer.putFloat(fogColor, 10.0f / 255.0f);
    Buffer.putFloat(fogColor, 1.0f);
    Buffer.flip(fogColor);

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.5f, 0.8f, 1.0f, 0.0f);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0, 1024.0 / 768.0, 0.05, 1000.0);

    Buffer.clear(viewportBuffer);
    glGetInteger(GL_VIEWPORT, viewportBuffer);
    Buffer.flip(viewportBuffer);
    Buffer.setLimit(viewportBuffer, 16);
    gluPickMatrix(512.0, 384.0, 5.0, 5.0, viewportBuffer);

    Buffer.clear(selectBuffer);
    glSelectBuffer(selectBuffer);
    glRenderMode(GL_SELECT);
    glRenderMode(GL_RENDER);

    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, -10.0f);
    glFogf(GL_FOG_END, 20.0f);
    glFog(GL_FOG_COLOR, fogColor);

    FloatBuffer *vertices = BufferUtils.createFloatBuffer(12);
    FloatBuffer *uvs = BufferUtils.createFloatBuffer(8);
    FloatBuffer *colors = BufferUtils.createFloatBuffer(12);

    glVertexPointer(3, 0, vertices);
    glTexCoordPointer(2, 0, uvs);
    glColorPointer(3, 0, colors);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glDrawArrays(GL_QUADS, 0, 4);

    GLuint texture = glGenTextures();
    glBindTexture(GL_TEXTURE_2D, texture);
    glDeleteTextures(texture);

    (void)Keyboard.isKeyDown(Keyboard.KEY_ESCAPE);
    (void)Keyboard.isKeyDown(Keyboard.KEY_W);
    (void)Keyboard.isKeyDown(Keyboard.KEY_A);
    (void)Keyboard.isKeyDown(Keyboard.KEY_S);
    (void)Keyboard.isKeyDown(Keyboard.KEY_D);
    (void)Keyboard.isKeyDown(Keyboard.KEY_SPACE);
    (void)Keyboard.isKeyDown(Keyboard.KEY_UP);
    (void)Keyboard.isKeyDown(Keyboard.KEY_DOWN);
    (void)Keyboard.isKeyDown(Keyboard.KEY_LEFT);
    (void)Keyboard.isKeyDown(Keyboard.KEY_RIGHT);

    (void)Mouse.getDX();
    (void)Mouse.getDY();
    while (Mouse.next()) {
        (void)Mouse.getEventButton();
        (void)Mouse.getEventButtonState();
    }

    while (Keyboard.next()) {
        (void)Keyboard.getEventKey();
        (void)Keyboard.getEventKeyState();
    }

    BufferUtils.destroy(colors);
    BufferUtils.destroy(uvs);
    BufferUtils.destroy(vertices);
    BufferUtils.destroy(selectBuffer);
    BufferUtils.destroy(viewportBuffer);
    BufferUtils.destroy(fogColor);

    Mouse.destroy();
    Keyboard.destroy();
    Display.destroy();

    delete mode;
}

int main() {
    /* Compile contract only. Do not create a window in CI. */
    if (false) compile_rubydung_surface();
    return 0;
}
