// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#include "encoders.h"
#include "bepo_table.h"

bool scroll_horizontal = false;
bool pc_locked         = false;

bool encoders_process_record(uint16_t keycode, keyrecord_t *record) {
    if (keycode == RIGHT_PUSH) {
        // Decided on release: tap.count tells a tap from a hold.
        if (!record->event.pressed) {
            if (record->tap.count) {
                scroll_horizontal = !scroll_horizontal;
            } else {
                tap_code16(LGUI(BE_L));
                pc_locked = true;
            }
        }
        return false;
    }

    if (record->event.pressed) {
        pc_locked = false;
    }

    switch (keycode) {
        case SCRL_UP:
            if (record->event.pressed) {
                tap_code16(scroll_horizontal ? MS_WHLR : MS_WHLU);
            }
            return false;
        case SCRL_DN:
            if (record->event.pressed) {
                tap_code16(scroll_horizontal ? MS_WHLL : MS_WHLD);
            }
            return false;
        default:
            return true;
    }
}
