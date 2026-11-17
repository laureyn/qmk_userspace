// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#include "layers.h"
#include "keymap.h"

// A locked layer ends after this time with no key pressed (D-40, FR-B0e).
#define LOCKED_LAYER_TIMEOUT 60000 // ms

static uint8_t  keys_held      = 0;
static uint32_t last_key_time  = 0;
static bool     sym_held       = false;
static bool     num_held       = false;
static bool     unlock_pending = false;

// A tap on SYM or NUM while MEDIA is locked clears every layer, not only the one tapped (6.4).
// That tap is consumed here, so QMK does not toggle the other layer back on.
bool layer_track_keys(uint16_t keycode, keyrecord_t *record) {
    if (keycode != TT(SYM) && keycode != TT(NUM)) {
        return true;
    }

    bool pressed = record->event.pressed;
    if (pressed) {
        unlock_pending = get_highest_layer(layer_state) == MEDIA && !sym_held && !num_held;
    }
    if (keycode == TT(SYM)) {
        sym_held = pressed;
    } else {
        num_held = pressed;
    }

    if (!unlock_pending) {
        return true;
    }
    if (!pressed) {
        unlock_pending = false;
        layer_clear();
    }
    return false;
}

// MEDIA is locked only when both SYM and NUM are locked. LOCK is not shown while a key is held (6.6).
bool layer_top_locked(void) {
    if (keys_held > 0) {
        return false;
    }
    switch (get_highest_layer(layer_state)) {
        case SYM:
            return !sym_held;
        case NUM:
            return !num_held;
        case MEDIA:
            return !sym_held && !num_held;
        default:
            return false;
    }
}

void layer_timeout_key(keyrecord_t *record) {
    if (record->event.pressed) {
        keys_held++;
    } else if (keys_held > 0) {
        keys_held--;
    }
    last_key_time = timer_read32();
}

// Holding a layer key never ends the layer: the timeout only counts when no key is held.
void layer_timeout_task(void) {
    if (keys_held == 0 && get_highest_layer(layer_state) > BASE && timer_elapsed32(last_key_time) > LOCKED_LAYER_TIMEOUT) {
        layer_clear();
    }
}
