// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#include "shift_key.h"
#include "caps_word.h"

// The Shift lock (double tap): Shift stays down until the key is tapped again.
static bool shift_locked = false;

// Runs once the Shift key's taps are known (held, single tap, double tap).
void shift_key_finished(tap_dance_state_t *state, void *user_data) {
    if (state->pressed) {
        // Held, or another key was pressed while it is down: Shift while held.
        register_mods(MOD_BIT(KC_LSFT));
    } else if (state->count == 1) {
        // Single tap: Caps Word, or end the Shift lock if it is on.
        if (shift_locked) {
            shift_locked = false;
            unregister_mods(MOD_BIT(KC_LSFT));
        } else {
            caps_word_toggle();
        }
    } else {
        // Double tap: Shift lock.
        shift_locked = true;
        register_mods(MOD_BIT(KC_LSFT));
    }
}

// Runs when the key is released: Shift ends with the key, unless it is locked.
void shift_key_reset(tap_dance_state_t *state, void *user_data) {
    if (!shift_locked) {
        unregister_mods(MOD_BIT(KC_LSFT));
    }
}
