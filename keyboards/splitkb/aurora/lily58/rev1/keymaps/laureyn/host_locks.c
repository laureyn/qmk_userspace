// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#include QMK_KEYBOARD_H

// Caps Lock on the PC changes what the digit row types on a Belgian layout, so it is
// always turned off (FR-A6). The LED callback only notes the state; the key is tapped
// from housekeeping, so the host's answer does not run back into the callback.
static bool caps_off_pending = false;

bool led_update_user(led_t led_state) {
    caps_off_pending = led_state.caps_lock;
    return true;
}

void housekeeping_task_user(void) {
    if (caps_off_pending) {
        caps_off_pending = false;
        tap_code(KC_CAPS);
    }
}
