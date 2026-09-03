#include <lwcgl/lwcgl.h>

int main()
{
    /* This must be safe before Display.create(): the swap-only helper is a
     * lightweight update(false)-style primitive that simply no-ops without a
     * native window. */
    lwcglDisplayUpdateNoMessages();
    return 0;
}
