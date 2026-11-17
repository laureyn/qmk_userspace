// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#pragma once
#include QMK_KEYBOARD_H

// Turns Caps Lock off at the PC when it was reported on (call from housekeeping).
void caps_lock_task(void);

// Turns Num Lock on at the PC if it is off (needed for Alt codes).
void ensure_num_lock(void);
