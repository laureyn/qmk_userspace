// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#pragma once
#include QMK_KEYBOARD_H

// Typing statistics since the last reset (D-34, NFR-7).
typedef struct {
    uint32_t chars;
    uint32_t backspaces;
} stats_t;

// Returns false when the key was handled (STATS_RESET).
bool stats_process_record(uint16_t keycode, keyrecord_t *record);

void stats_load(void);
void stats_task(void);

// Counts kept by the USB half (what is sent to the other half).
const stats_t *stats_local(void);

// Counts to show on this half: local on the USB half, received on the other.
const stats_t *stats_shown(void);

// Called on the half without the USB cable with the counts from the USB half.
void stats_from_master(const stats_t *stats);
