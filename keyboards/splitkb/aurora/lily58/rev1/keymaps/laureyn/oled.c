// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#include QMK_KEYBOARD_H

// Placeholder text on both screens (DEV-99). The board calls this first;
// returning false skips the board's own logo and status drawing.
bool oled_task_user(void) {
    oled_write_ln_P(is_keyboard_left() ? PSTR("LEFT") : PSTR("RIGHT"), false);
    return false;
}
