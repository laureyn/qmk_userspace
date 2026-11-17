// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#include "bepo_table.h"

// Caps Word stays on for letters, `-` (it becomes `_`) and Backspace. Any other key ends it.
// QMK's default checks US positions, which on a Belgian layout would stop at the `m` key,
// so the keys are listed here as Belgian keycodes.
bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        case BE_A:
        case BE_B:
        case BE_C:
        case BE_D:
        case BE_E:
        case BE_F:
        case BE_G:
        case BE_H:
        case BE_I:
        case BE_J:
        case BE_K:
        case BE_L:
        case BE_M:
        case BE_N:
        case BE_O:
        case BE_P:
        case BE_Q:
        case BE_R:
        case BE_S:
        case BE_T:
        case BE_U:
        case BE_V:
        case BE_W:
        case BE_X:
        case BE_Y:
        case BE_Z:
        case BP_MINS:
        case KC_BSPC:
            return true;
        default:
            return false;
    }
}
