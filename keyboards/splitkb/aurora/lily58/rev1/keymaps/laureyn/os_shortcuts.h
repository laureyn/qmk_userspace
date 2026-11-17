// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#pragma once
#include QMK_KEYBOARD_H
#include "bepo_table.h"

// Handles the keys whose shortcut depends on the OS (Redo). Returns false when the key was handled.
bool os_shortcuts_process_record(uint16_t keycode, keyrecord_t *record);
