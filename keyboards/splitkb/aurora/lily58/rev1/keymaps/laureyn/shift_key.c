// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#include "shift_key.h"

// The Shift lock (double tap): Shift stays down until the key is tapped again.
static bool shift_locked = false;
// One-shot Shift (single tap): Shift for the next key pressed only.
static bool shift_once = false;
// The one-shot Shift is on the key being pressed, so it ends when that key is released.
static bool shift_once_in_use = false;

// Runs once the Shift key's taps are known (held, single tap, double tap).
void shift_key_finished(tap_dance_state_t *state, void *user_data) {
    if (state->pressed) {
        // Held, or another key was pressed while it is down: Shift while held.
        register_mods(MOD_BIT(KC_LSFT));
    } else if (state->count == 1) {
        // Single tap: Shift for the next key, or end the Shift lock if it is on.
        if (shift_locked) {
            shift_locked = false;
            unregister_mods(MOD_BIT(KC_LSFT));
        } else {
            shift_once = true;
            register_mods(MOD_BIT(KC_LSFT));
        }
    } else {
        // Double tap: Shift lock.
        shift_once   = false;
        shift_locked = true;
        register_mods(MOD_BIT(KC_LSFT));
    }
}

// Runs when the key is released: Shift ends with the key, unless it is locked or one-shot.
void shift_key_reset(tap_dance_state_t *state, void *user_data) {
    if (!shift_locked && !shift_once) {
        unregister_mods(MOD_BIT(KC_LSFT));
    }
}

// Runs after every key event. The first key pressed after a single tap gets the Shift,
// and the Shift ends when that key is released.
void shift_key_post_record(uint16_t keycode, keyrecord_t *record) {
    if (keycode == TD(TD_SHIFT)) {
        return;
    }
    if (record->event.pressed) {
        if (shift_once) {
            shift_once        = false;
            shift_once_in_use = true;
        }
    } else if (shift_once_in_use) {
        shift_once_in_use = false;
        if (!shift_locked) {
            unregister_mods(MOD_BIT(KC_LSFT));
        }
    }
}

bool shift_key_locked(void) {
    return shift_locked;
}

bool shift_key_once(void) {
    return shift_once;
}
