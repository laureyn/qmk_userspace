// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#include "bepo_dead.h"

// The dead-key keys are numbered from BP_ECIRC in the enum, so held[] is indexed by that number.
#define DEAD_KEY_COUNT (BP_GRAVE - BP_ECIRC + 1)

// For keys with a normal tap: the keycode held while the key is down (0 = none).
static uint16_t held[DEAD_KEY_COUNT];

void send_dead_key_letter(uint16_t dead, uint16_t letter, bool shifted) {
    uint8_t saved_mods = get_mods();
    del_mods(MOD_MASK_SHIFT); // the dead key must not get Shift

    tap_code16(dead);
    tap_code16(shifted ? S(letter) : letter);

    set_mods(saved_mods);
}

// Key with a normal tap and a dead-key Shift output (é, è, à, ù, %).
static bool dead_or_tap(uint16_t keycode, uint16_t tap, uint16_t dead, uint16_t letter, keyrecord_t *record) {
    uint8_t index = keycode - BP_ECIRC;
    if (record->event.pressed) {
        if (get_mods() & MOD_MASK_SHIFT) {
            // Shift + "%" gives the lone grave, which is sent without a letter (KC_SPC)
            send_dead_key_letter(dead, letter, letter != KC_SPC);
        } else {
            register_code16(tap);
            held[index] = tap;
        }
    } else if (held[index]) {
        unregister_code16(held[index]);
        held[index] = 0;
    }
    return false;
}

// Key whose tap and Shift output are both dead-key sequences (ê, Ê).
static bool dead_only(uint16_t dead, uint16_t letter, keyrecord_t *record) {
    if (record->event.pressed) {
        send_dead_key_letter(dead, letter, get_mods() & MOD_MASK_SHIFT);
    }
    return false;
}

// Lone dead key (^ ~ `): only the dead key is sent, so the next letter can combine with it.
static bool lone_dead(uint16_t dead, keyrecord_t *record) {
    if (record->event.pressed) {
        uint8_t saved_mods = get_mods();
        del_mods(MOD_MASK_SHIFT); // the dead key must not get Shift
        tap_code16(dead);
        set_mods(saved_mods);
    }
    return false;
}

bool bepo_dead_process_record(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case BP_ECIRC:  return dead_only(BE_DCIR, BE_E, record);                     // ê / Ê
        case BP_EACUTE: return dead_or_tap(keycode, BE_EACU, BE_ACUT, BE_E, record); // é / É
        case BP_EGRAVE: return dead_or_tap(keycode, BE_EGRV, BE_GRV, BE_E, record); // è / È
        case BP_AGRAVE: return dead_or_tap(keycode, BE_AGRV, BE_GRV, BE_A, record); // à / À
        case BP_UGRAVE: return dead_or_tap(keycode, BE_UGRV, BE_GRV, BE_U, record); // ù / Ù
        case BP_PERC:   return dead_or_tap(keycode, BE_PERC, BE_GRV, KC_SPC, record); // % / `
        case BP_CIRC:   return lone_dead(BE_DCIR, record);                           // ^
        case BP_TILD:   return lone_dead(BE_TILD, record);                           // ~
        case BP_GRAVE:  return lone_dead(BE_GRV, record);                            // `
        default:        return true;
    }
}
