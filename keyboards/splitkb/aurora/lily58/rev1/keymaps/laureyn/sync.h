// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#pragma once
#include QMK_KEYBOARD_H
#include "stats.h"

// Keyboard state shown on the screens. The USB half works it out and sends it.
typedef struct {
    uint8_t layer;        // top layer (BASE, SYM, NUM, MEDIA)
    bool    layer_locked; // top layer is on without its key held
    bool    shift_locked;
    bool    shift_once;
    uint8_t os; // os_variant_t: OS_WINDOWS or OS_LINUX
    bool    oled_enabled;
} status_t;

// What the USB half sends to the other half, a few times per second.
typedef struct {
    status_t status;
    stats_t  stats;
} user_sync_t;

// Status to show on this half: local on the USB half, received on the other.
status_t status_shown(void);

void sync_task(void);
