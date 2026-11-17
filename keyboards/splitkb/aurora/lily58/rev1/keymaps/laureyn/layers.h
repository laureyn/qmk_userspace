// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#pragma once
#include QMK_KEYBOARD_H

// Call for every key event, to know when the keyboard was last used.
void layer_timeout_key(keyrecord_t *record);

// Call from housekeeping: a locked layer ends after an idle time with no key held.
void layer_timeout_task(void);

// Call for every key event: remembers whether the SYM and NUM keys are held.
void layer_track_keys(uint16_t keycode, keyrecord_t *record);

// True when the top layer is on but its key is not held (tap-locked).
bool layer_top_locked(void);
