// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#include "rgb.h"
#include "bepo_table.h"

// Switch one LED group (underglow or key light) on or off; the choice is saved (D-38, FR-E1).
static void toggle_led_group(led_flags_t group) {
    led_flags_t flags = rgb_matrix_get_flags() ^ group;
    rgb_matrix_set_flags(flags);
    if ((flags & (LED_FLAG_UNDERGLOW | LED_FLAG_KEYLIGHT)) == LED_FLAG_NONE) {
        // No effect draws any more, so clear the last colours.
        rgb_matrix_set_color_all(0, 0, 0);
    }
}

bool rgb_flags_process_record(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        return true;
    }
    switch (keycode) {
        case RGB_GLOW_TOG:
            toggle_led_group(LED_FLAG_UNDERGLOW);
            return false;
        case RGB_KEYS_TOG:
            toggle_led_group(LED_FLAG_KEYLIGHT);
            return false;
        default:
            return true;
    }
}
