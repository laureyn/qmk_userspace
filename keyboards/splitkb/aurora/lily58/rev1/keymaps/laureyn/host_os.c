// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#include "host_os.h"

// The override is one byte at the start of the user data block (see config.h).
// Anything unknown (e.g. after a settings reset) counts as auto.
#define OS_MODE_OFFSET 0

os_mode_t os_mode_get(void) {
    uint8_t saved = OS_MODE_AUTO;
    if (eeconfig_is_user_datablock_valid()) {
        eeconfig_read_user_datablock(&saved, OS_MODE_OFFSET, sizeof(saved));
    }
    return saved <= OS_MODE_LINUX ? (os_mode_t)saved : OS_MODE_AUTO;
}

void os_mode_set(os_mode_t mode) {
    uint8_t value = (uint8_t)mode;
    eeconfig_update_user_datablock(&value, OS_MODE_OFFSET, sizeof(value));
}

// Only Windows and Linux are supported (D-03, D-16). The override wins; without
// one, anything the keyboard cannot tell apart (still unsure, macOS, iOS) is Windows.
os_variant_t current_os(void) {
    switch (os_mode_get()) {
        case OS_MODE_WINDOWS:
            return OS_WINDOWS;
        case OS_MODE_LINUX:
            return OS_LINUX;
        default:
            break;
    }
    return detected_host_os() == OS_LINUX ? OS_LINUX : OS_WINDOWS;
}
