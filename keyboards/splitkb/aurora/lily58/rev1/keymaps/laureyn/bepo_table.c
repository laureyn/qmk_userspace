// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#include "bepo_table.h"

const bepo_entry_t bepo_table[] = {
    {BP_COMM, BE_COMM, BE_SCLN}
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
