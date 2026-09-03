#ifndef LWCGL_INPUT_STATE_H
#define LWCGL_INPUT_STATE_H

#include <string.h>

#define LWCGL_INPUT_KEY_CAPACITY 512

typedef struct LwcglInputState {
    unsigned char key_down[LWCGL_INPUT_KEY_CAPACITY];
    int close_requested;
} LwcglInputState;

static inline void lwcgl_input_reset(LwcglInputState *state) {
    if (!state) return;
    memset(state, 0, sizeof(*state));
}

static inline void lwcgl_input_set_key(LwcglInputState *state, int key, int down) {
    if (!state || key < 0 || key >= LWCGL_INPUT_KEY_CAPACITY) return;
    state->key_down[key] = down ? 1u : 0u;
}

static inline int lwcgl_input_key_down(const LwcglInputState *state, int key) {
    return state
        && key >= 0
        && key < LWCGL_INPUT_KEY_CAPACITY
        && state->key_down[key] != 0u;
}

static inline void lwcgl_input_request_close(LwcglInputState *state) {
    if (state) state->close_requested = 1;
}

static inline int lwcgl_input_close_requested(const LwcglInputState *state) {
    return state && state->close_requested != 0;
}

#endif
