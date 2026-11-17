// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#if __has_include("keymap.h")
#    include "keymap.h"
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    layer_timeout_key(record);
    if (!stats_process_record(keycode, record)) {
        return false;
    }
    if (!oled_process_record(keycode, record)) {
        return false;
    }
    if (!encoders_process_record(keycode, record)) {
        return false;
    }
    if (!rgb_flags_process_record(keycode, record)) {
        return false;
    }
    if (!settings_keys_process_record(keycode, record)) {
        return false;
    }
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

tap_dance_action_t tap_dance_actions[] = {
    [TD_SHIFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, shift_key_finished, shift_key_reset),
};

// SYM + NUM together = MEDIA (tri-layer, D-23, FR-B0).
layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, SYM, NUM, MEDIA);
}

void housekeeping_task_user(void) {
    caps_lock_task();
    layer_timeout_task();
    settings_reset_task();
    stats_task();
    sync_task();
}

// BASE (bépo 1.1 start, keymap-v2). Each key shows its tap character.
// Shift gives the second character of the key (see keymap-v2.md).
//
//   Esc  "    |    @    &    €   |   $    +    -    /    *    %
//   Tab  b    é    p    o    z   |   w    v    d    l    j    =
//   Shft a    u    i    e    ,   |   c    t    s    r    n    m
//   Ctrl à    y    x    .    k   EncL EncR '    q    g    h    f    Ctrl
//                   GUI  Alt  SYM  Spc    Ent  NUM  Bksp Shft
//
// Layer keys: 58 positions per layer, same order as BASE.
// Positions 42 and 43 are the left and right encoder push keys.
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT(KC_ESC, BP_DQUO, BP_PIPE, BP_AT, BP_AMPR, BP_EURO, BP_DLR, BP_PLUS, BP_MINS, BP_SLSH, BP_ASTR, BP_PERC, KC_TAB, BE_B, BP_EACUTE, BE_P, BE_O, BE_Z, BE_W, BE_V, BE_D, BE_L, BE_J, BP_EQL, TD(TD_SHIFT), BE_A, BE_U, BE_I, BE_E, BP_COMM, BE_C, BE_T, BE_S, BE_R, BE_N, BE_M, KC_LCTL, BP_AGRAVE, BE_Y, BE_X, BP_DOT, BE_K, KC_MUTE, RIGHT_PUSH, BP_QUOT, BE_Q, BE_G, BE_H, BE_F, KC_RCTL, KC_LGUI, KC_LALT, TT(SYM), KC_SPC, KC_ENT, TT(NUM), KC_BSPC, TD(TD_SHIFT)),

    [SYM] = LAYOUT(
        KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12,
        _______, BP_TILD, BP_EGRAVE, BP_ECIRC, BP_UGRAVE, BP_CIRC, BE_EXLM, BE_BSLS, BE_SCLN, BE_COLN, BE_HASH, BP_GRAVE,
        _______, BE_LABK, BE_LBRC, BE_LCBR, BE_LPRN, BE_UNDS, BP_CCED, BE_RPRN, BE_RCBR, BE_RBRC, BE_RABK, XXXXXXX,
        _______, C(BE_Z), C(BE_X), C(BE_C), C(BE_V), OS_REDO, _______, RIGHT_PUSH, BE_QUES, BP_OE, BP_ELLIPSIS, XXXXXXX, XXXXXXX, _______,
        _______, _______, TT(SYM), _______, _______, TT(NUM), KC_DEL, _______),

    [NUM] = LAYOUT(
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, BP_SLSH, KC_PSCR, KC_INS, KC_DEL, XXXXXXX, XXXXXXX, XXXXXXX,
        _______, XXXXXXX, BE_7, BE_8, BE_9, BP_ASTR, KC_PGUP, KC_HOME, KC_UP, KC_END, XXXXXXX, XXXXXXX,
        _______, BP_EQL, BE_4, BE_5, BE_6, BP_PLUS, KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX, XXXXXXX,
        _______, BP_COMM, BE_1, BE_2, BE_3, BP_MINS, _______, RIGHT_PUSH, XXXXXXX, XXXXXXX, KC_APP, XXXXXXX, XXXXXXX, _______,
        BE_0, BP_DOT, TT(SYM), _______, _______, TT(NUM), _______, _______),

    [MEDIA] = LAYOUT(
        XXXXXXX, SETTINGS_RESET, STATS_RESET, XXXXXXX, XXXXXXX, XXXXXXX, OSKEY_AUTO, OSKEY_WINDOWS, OSKEY_LINUX, XXXXXXX, XXXXXXX, XXXXXXX,
        _______, RM_TOGG, RGB_GLOW_TOG, RGB_KEYS_TOG, OLED_TOG, XXXXXXX, XXXXXXX, XXXXXXX, KC_MSTP, XXXXXXX, XXXXXXX, XXXXXXX,
        _______, RM_HUEU, RM_SATU, RM_VALU, RM_SPDU, RM_NEXT, XXXXXXX, KC_MPRV, KC_MPLY, KC_MNXT, XXXXXXX, XXXXXXX,
        _______, RM_HUED, RM_SATD, RM_VALD, RM_SPDD, RM_PREV, _______, RIGHT_PUSH, XXXXXXX, KC_SLEP, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, TT(SYM), XXXXXXX, XXXXXXX, TT(NUM), XXXXXXX, XXXXXXX),
};

// Encoders per layer: ENCODER_CCW_CW(counter-clockwise, clockwise).
// Left = volume (brightness on MEDIA). Right = scroll (RGB value on MEDIA).
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [BASE]  = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(SCRL_DN, SCRL_UP)},
    [SYM]   = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(SCRL_DN, SCRL_UP)},
    [NUM]   = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(SCRL_DN, SCRL_UP)},
    [MEDIA] = {ENCODER_CCW_CW(KC_BRID, KC_BRIU), ENCODER_CCW_CW(RM_VALD, RM_VALU)},
};
