#include <lwcgl/lwcgl.h>

#include <cstdio>

int main() {
    Display.setDisplayMode(new DisplayMode(1024, 768));
    Display.setTitle("rd-132328");

    if (Display.create() != 0) {
        std::fprintf(stderr, "Display.create failed: %s\n",
                     lwcglGetLastError() ? lwcglGetLastError() : "unknown error");
        return 1;
    }

    if (Keyboard.create() != 0 || Mouse.create() != 0) {
        std::fprintf(stderr, "input create failed: %s\n",
                     lwcglGetLastError() ? lwcglGetLastError() : "unknown error");
        Display.destroy();
        return 1;
    }

    Mouse.setGrabbed(true);

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.5F, 0.8F, 1.0F, 0.0F);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);

    while (!Keyboard.isKeyDown(Keyboard.KEY_ESCAPE) &&
           !Display.isCloseRequested()) {
        const int dx = Mouse.getDX();
        const int dy = Mouse.getDY();
        (void)dx;
        (void)dy;

        while (Mouse.next()) {
            if (Mouse.getEventButton() == 0 && Mouse.getEventButtonState()) {
                std::puts("left mouse button");
            }
        }

        while (Keyboard.next()) {
            if (Keyboard.getEventKey() == Keyboard.KEY_RETURN &&
                Keyboard.getEventKeyState()) {
                std::puts("enter");
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Display.update();
    }

    Mouse.destroy();
    Keyboard.destroy();
    Display.destroy();
    return 0;
}
