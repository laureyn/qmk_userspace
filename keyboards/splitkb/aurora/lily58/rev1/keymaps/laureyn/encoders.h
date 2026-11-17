// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#pragma once
#include QMK_KEYBOARD_H

// Right encoder push: tap = switch scroll axis, short hold = lock the PC (E4.2).
#define RIGHT_PUSH LT(0, KC_NO)

extern bool scroll_horizontal;
extern bool pc_locked;

bool encoders_process_record(uint16_t keycode, keyrecord_t *record);
