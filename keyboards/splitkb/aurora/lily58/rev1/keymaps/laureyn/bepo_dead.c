// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#include "bepo_dead.h"

void send_dead_key_letter(uint16_t dead, uint16_t letter, bool shifted) {
    uint8_t saved_mods = get_mods();
    del_mods(MOD_MASK_SHIFT); // the dead key must not get Shift

    tap_code16(dead);
    tap_code16(shifted ? S(letter) : letter);

    set_mods(saved_mods);
}

bool bepo_dead_process_record(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case BP_ECIRC:  // ê: dead ^, then e
            if (record->event.pressed) {
                send_dead_key_letter(BE_DCIR, BE_E, false);
            }
            return false;
        case BP_EACUTE: // É: dead ´, then Shift+e
            if (record->event.pressed) {
                send_dead_key_letter(BE_ACUT, BE_E, true);
            }
            return false;
        default:
            return true;
    }
}
