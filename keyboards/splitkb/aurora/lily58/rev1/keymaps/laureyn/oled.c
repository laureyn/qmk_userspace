// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#include "sync.h"

// Placeholder text on both screens (DEV-99). The board calls this first;
// returning false skips the board's own logo and status drawing.
// Left screen: "CAPS" under the name while Caps Word is on (DEV-101).
bool oled_task_user(void) {
    if (is_keyboard_left()) {
        oled_write_ln_P(PSTR("LEFT"), false);
        oled_write_ln_P(caps_word_shown() ? PSTR("CAPS") : PSTR("    "), false);
    } else {
        oled_write_ln_P(PSTR("RIGHT"), false);
    }
    return false;
}
