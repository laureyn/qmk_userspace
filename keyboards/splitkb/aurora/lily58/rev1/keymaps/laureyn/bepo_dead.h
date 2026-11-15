// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#pragma once
#include QMK_KEYBOARD_H
#include "bepo_table.h"

// Types a Belgian dead key, then a letter, so that accented letters like ê and É come out.
void send_dead_key_letter(uint16_t dead, uint16_t letter, bool shifted);

// Handles the dead-key letters. Returns false when the key was handled.
bool bepo_dead_process_record(uint16_t keycode, keyrecord_t *record);
