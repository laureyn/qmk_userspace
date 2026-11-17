// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#pragma once
#include QMK_KEYBOARD_H

// Tap dance index of the Shift keys (used as TD(TD_SHIFT) on BASE).
enum tap_dance_keys {
    TD_SHIFT,
};

void shift_key_finished(tap_dance_state_t *state, void *user_data);
void shift_key_reset(tap_dance_state_t *state, void *user_data);
