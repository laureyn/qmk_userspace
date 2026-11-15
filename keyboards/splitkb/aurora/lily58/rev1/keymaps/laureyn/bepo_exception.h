// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#pragma once
#include QMK_KEYBOARD_H
#include "bepo_table.h"

// Handles the exception characters (Ç and …) with the Linux method. Returns false when the key was handled.
bool bepo_exception_process_record(uint16_t keycode, keyrecord_t *record);
