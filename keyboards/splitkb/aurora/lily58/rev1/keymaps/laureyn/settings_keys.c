// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#include "settings_keys.h"
#include "bepo_table.h"

// The reset is guarded: it only happens when the key is held for this long (keymap-v2, footnote 4).
#define SETTINGS_RESET_HOLD 2000 // ms

static bool     reset_held  = false;
static uint32_t reset_start = 0;

bool settings_keys_process_record(uint16_t keycode, keyrecord_t *record) {
    if (keycode != SETTINGS_RESET) {
        return true;
    }
    reset_held = record->event.pressed;
    if (reset_held) {
        reset_start = timer_read32();
    }
    return false;
}

void settings_reset_task(void) {
    if (reset_held && timer_elapsed32(reset_start) >= SETTINGS_RESET_HOLD) {
        reset_held = false;
        eeconfig_init();
        soft_reset_keyboard();
    }
}
