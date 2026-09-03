#include <lwcgl/lwcgl.h>

#include <cassert>

int main()
{
    assert(Display.updateNoMessages != nullptr);

    /* Both entry points must be safe before Display.create(): the swap-only
     * update primitive simply no-ops without a native window. */
    Display.updateNoMessages();
    lwcglDisplayUpdateNoMessages();
    return 0;
}
