// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#include "layers.h"
#include "keymap.h"

// A locked layer ends after this time with no key pressed (D-40, FR-B0e).
#define LOCKED_LAYER_TIMEOUT 60000 // ms

static uint8_t  keys_held     = 0;
static uint32_t last_key_time = 0;

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
