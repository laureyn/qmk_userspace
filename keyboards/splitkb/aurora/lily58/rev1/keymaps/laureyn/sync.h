// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#pragma once
#include QMK_KEYBOARD_H

// Caps Word is tracked on the half with the USB cable. This tells either half
// whether Caps Word is on (DEV-102).
bool caps_word_shown(void);

void sync_task(void);
