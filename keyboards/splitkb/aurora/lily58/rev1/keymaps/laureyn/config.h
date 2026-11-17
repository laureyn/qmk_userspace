// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#pragma once

// TT keys: hold = layer while held, one tap = layer locked, tap again = unlocked (D-23).
#define TAPPING_TOGGLE 1

// Both halves share the activity time, so the screens and LEDs go off together (DEV-107, DEV-109).
#define SPLIT_ACTIVITY_ENABLE

// Custom sync from the USB half to the other half: Caps Word, OLED choice and counts (DEV-102, DEV-104).
#define SPLIT_TRANSACTION_IDS_USER USER_SYNC_STATE
// Typing speed (characters per minute) is synced by QMK; the counts use the user sync above.
#define SPLIT_WPM_ENABLE

// Typing counts are saved in the user data block (stats_t, 8 bytes).
#define EECONFIG_USER_DATA_SIZE 8

// RGB: on at first boot with one effect; brightness stays at the board's cap (DEV-37).
#define RGB_MATRIX_DEFAULT_ON true
#define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_CYCLE_LEFT_RIGHT
// LEDs off after 5 min idle and while the PC sleeps; back at the next key (DEV-109).
#define RGB_MATRIX_TIMEOUT 300000
#define RGB_MATRIX_SLEEP

// The screens stay on long enough for the 5-minute idle animation (DEV-100).
#define OLED_TIMEOUT 1800000
