// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#pragma once
#include QMK_KEYBOARD_H
#include "stats.h"

// What the USB half sends to the other half, a few times per second.
typedef struct {
    bool    caps_word;
    bool    oled_enabled;
    stats_t stats;
} user_sync_t;

// Caps Word is tracked on the half with the USB cable (DEV-102).
bool caps_word_shown(void);

void sync_task(void);
