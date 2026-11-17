// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#pragma once

#include QMK_KEYBOARD_H
#include "os_detection.h"

// The OS chosen by hand from MEDIA, or auto (follow the detection).
typedef enum {
    OS_MODE_AUTO,
    OS_MODE_WINDOWS,
    OS_MODE_LINUX,
} os_mode_t;

// The saved override, kept in the keyboard's memory across unplugging.
os_mode_t os_mode_get(void);
void      os_mode_set(os_mode_t mode);

// The OS the keyboard types for: OS_WINDOWS or OS_LINUX.
os_variant_t current_os(void);
