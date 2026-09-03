extern "C" {
#include "../src/input_state.h"
}

#include <cassert>

int main()
{
    LwcglInputState state = {};

    assert(!lwcgl_input_key_down(&state, 1));
    lwcgl_input_set_key(&state, 1, 1);
    assert(lwcgl_input_key_down(&state, 1));
    lwcgl_input_set_key(&state, 1, 0);
    assert(!lwcgl_input_key_down(&state, 1));

    assert(!lwcgl_input_close_requested(&state));
    lwcgl_input_request_close(&state);
    assert(lwcgl_input_close_requested(&state));

    lwcgl_input_reset(&state);
    assert(!lwcgl_input_key_down(&state, 1));
    assert(!lwcgl_input_close_requested(&state));

    return 0;
}
