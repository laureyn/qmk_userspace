// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#pragma once
#include QMK_KEYBOARD_H
#include "keymap_belgian.h"
#include <stdint.h>

enum custom_keycodes {
    BP_COMM = SAFE_RANGE
};

typedef struct {
    uint16_t custom;
    uint16_t tap;
    uint16_t shift;
} bepo_entry_t;

extern const bepo_entry_t	bepo_table[];
extern const uint8_t		bepo_table_count;

bool bepo_process_record(uint16_t keycode, keyrecord_t *record);
