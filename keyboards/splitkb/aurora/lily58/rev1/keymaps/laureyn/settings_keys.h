// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#pragma once
#include QMK_KEYBOARD_H

// Handles the guarded settings reset key. Returns false when the key was handled.
bool settings_keys_process_record(uint16_t keycode, keyrecord_t *record);

// Call from housekeeping: clears the saved settings after the key was held for 2 s.
void settings_reset_task(void);
