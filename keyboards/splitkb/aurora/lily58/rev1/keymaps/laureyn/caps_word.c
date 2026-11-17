// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#include "bepo_table.h"

// Caps Word stays on for letters, the digit row (their Shift character is the digit) and
// Backspace. Any other key ends it. Letters and digit-row keys get Shift from Caps Word
// (weak Shift), so bepo_table.c sees it. QMK's default checks US positions, which on a
// Belgian layout would stop at the `m` key, so the keys are listed here as Belgian keycodes.
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
        case BP_DQUO:
        case BP_PIPE:
        case BP_AT:
        case BP_AMPR:
        case BP_EURO:
        case BP_DLR:
        case BP_PLUS:
        case BP_MINS:
        case BP_SLSH:
        case BP_ASTR:
            add_weak_mods(MOD_BIT(KC_LSFT));
            return true;
        case KC_BSPC:
            return true;
        default:
            return false;
    }
}
