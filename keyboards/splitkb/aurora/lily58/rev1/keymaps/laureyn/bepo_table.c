// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#include "bepo_table.h"

const bepo_entry_t bepo_table[] = {
    {BP_COMM, BE_COMM, BE_SCLN}
};

const uint8_t bepo_table_count = sizeof(bepo_table) / sizeof(bepo_table[0]);

bool bepo_process_record(uint16_t keycode, keyrecord_t *record) {
    return true;
}
