// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#pragma once
#include QMK_KEYBOARD_H

// The user's screen on/off choice (MEDIA OLED key). The idle timeout still turns the screens off.
bool oled_wanted(void);
void oled_set_wanted(bool on);

bool oled_process_record(uint16_t keycode, keyrecord_t *record);
