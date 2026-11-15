// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#include "bepo_table.h"

// One row per key whose Shift character differs from what Belgian gives natively.
// Comment: tap character, then Shift character.
const bepo_entry_t bepo_table[] = {
    {BP_COMM, BE_COMM, BE_SCLN}, // ,  ;
    {BP_DQUO, BE_DQUO, BE_1},    // "  1
    {BP_PIPE, BE_PIPE, BE_2},    // |  2
    {BP_AT,   BE_AT,   BE_3},    // @  3
    {BP_AMPR, BE_AMPR, BE_4},    // &  4
    {BP_EURO, BE_EURO, BE_5},    // €  5
    {BP_DLR,  BE_DLR,  BE_6},    // $  6
    {BP_PLUS, BE_PLUS, BE_7},    // +  7
    {BP_MINS, BE_MINS, BE_8},    // -  8
    {BP_SLSH, BE_SLSH, BE_9},    // /  9
    {BP_ASTR, BE_ASTR, BE_0},    // *  0
    {BP_EQL,  BE_EQL,  BE_DEG},  // =  °
    {BP_DOT,  BE_DOT,  BE_COLN}, // .  :
    {BP_QUOT, BE_QUOT, BE_QUES}, // '  ?
    {BP_OE,   ALGR(BE_O), ALGR(S(BE_O))}, // œ  Œ
};

const uint8_t bepo_table_count = sizeof(bepo_table) / sizeof(bepo_table[0]);

// While a table key is held, remember what was sent for it's row,
// and which Shift (if any) was taken out of the report.
static uint16_t sent_keycode[sizeof(bepo_table) / sizeof(bepo_table[0])];
static uint8_t removed_mods[sizeof(bepo_table) / sizeof(bepo_table[0])];

bool bepo_process_record(uint16_t keycode, keyrecord_t *record) {
    for (uint8_t i = 0; i < bepo_table_count; i++) {
        if (bepo_table[i].custom != keycode) {
            continue;
        }

        if (record->event.pressed) {
            // Take Shift out of the report, so the table decides the character.
            removed_mods[i] = get_mods() & MOD_MASK_SHIFT;
            del_mods(MOD_MASK_SHIFT);

            sent_keycode[i] = removed_mods[i] ? bepo_table[i].shift : bepo_table[i].tap;
            register_code16(sent_keycode[i]);
        } else {
            // Release what was pressed, then give back the Shift we took.
            unregister_code16(sent_keycode[i]);
            add_mods(removed_mods[i]);

            sent_keycode[i] = 0;
            removed_mods[i] = 0;
        }
        return false;
    }
    return true;
}
