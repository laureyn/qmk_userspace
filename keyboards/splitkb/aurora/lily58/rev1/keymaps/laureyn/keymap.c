// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#if __has_include("keymap.h")
#    include "keymap.h"
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!bepo_process_record(keycode, record)) {
        return false;
    }
    if (!bepo_dead_process_record(keycode, record)) {
        return false;
    }
    if (!bepo_exception_process_record(keycode, record)) {
        return false;
    }
    return os_shortcuts_process_record(keycode, record);
}

// BASE (bépo 1.1 start, keymap-v2). Each key shows its tap character.
// Shift gives the second character of the key (see keymap-v2.md).
//
//   Esc  "    |    @    &    €   |   $    +    -    /    *    %
//   Tab  b    é    p    o    z   |   w    v    d    l    j    =
//   Shft a    u    i    e    ,   |   c    t    s    r    n    m
//   Ctrl à    y    x    .    k   EncL EncR '    q    g    h    f    Ctrl
//                   GUI  Alt  SYM  Spc    Ent  NUM  Bksp Shft
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {[BASE]  = LAYOUT(KC_ESC, BP_DQUO, BP_PIPE, BP_AT, BP_AMPR, BP_EURO, BP_DLR, BP_PLUS, BP_MINS, BP_SLSH, BP_ASTR, BP_PERC, KC_TAB, BE_B, BP_EACUTE, BE_P, BE_O, BE_Z, BE_W, BE_V, BE_D, BE_L, BE_J, BP_EQL, KC_LSFT, BE_A, BE_U, BE_I, BE_E, BP_COMM, BE_C, BE_T, BE_S, BE_R, BE_N, BE_M, KC_LCTL, BP_AGRAVE, BE_Y, BE_X, BP_DOT, BE_K, XXXXXXX, XXXXXXX, BP_QUOT, BE_Q, BE_G, BE_H, BE_F, KC_RCTL, KC_LGUI, KC_LALT, MO(1), KC_SPC, KC_ENT, MO(2), KC_BSPC, KC_RSFT),
                                                              [SYM]   = LAYOUT(_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_GRV, KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_TILD, _______, _______, _______, _______, _______, OS_REDO, _______, _______, XXXXXXX, KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, _______, _______, _______, _______, _______, MO(3), _______, _______),
                                                              [NUM]   = LAYOUT(_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, _______, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, XXXXXXX, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, XXXXXXX, _______, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, _______, _______, KC_PLUS, KC_MINS, KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS, _______, _______, MO(3), _______, _______, _______, _______, _______),
                                                              [MEDIA] = LAYOUT(BP_OE, BP_CCED, BP_ELLIPSIS, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RM_TOGG, RM_HUEU, RM_SATU, RM_VALU, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RM_NEXT, RM_HUED, RM_SATD, RM_VALD, _______, _______, _______, _______, _______, _______, _______, _______)};
