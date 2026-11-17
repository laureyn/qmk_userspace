// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#pragma once
#include QMK_KEYBOARD_H

// Call for every key event, to know when the keyboard was last used.
void layer_timeout_key(keyrecord_t *record);

// Call from housekeeping: a locked layer ends after an idle time with no key held.
void layer_timeout_task(void);
