// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#include "host_locks.h"

// The lock lights as last reported by the PC.
static led_t host_leds;

// Caps Lock on the PC changes what the digit row types on a Belgian layout, so it is
// always turned off (FR-A6). The LED callback only notes the state; the key is tapped
// from housekeeping, so the host's answer does not run back into the callback.
static bool caps_off_pending = false;

bool led_update_user(led_t led_state) {
    host_leds        = led_state;
    caps_off_pending = led_state.caps_lock;
    return true;
}

void housekeeping_task_user(void) {
    if (caps_off_pending) {
        caps_off_pending = false;
        tap_code(KC_CAPS);
    }
}

// Num Lock must be on for keypad digits (Alt codes). The state is set to on straight
// after the tap, so a second call before the PC reports back does not turn it off again.
void ensure_num_lock(void) {
    if (!host_leds.num_lock) {
        tap_code(KC_NUM);
        host_leds.num_lock = true;
    }
}
