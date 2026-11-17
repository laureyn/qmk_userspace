// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#include "host_os.h"

// The override is the whole 32-bit user setting. Anything unknown (e.g. after a
// settings reset) counts as auto.
os_mode_t os_mode_get(void) {
    uint32_t saved = eeconfig_read_user();
    return saved <= OS_MODE_LINUX ? (os_mode_t)saved : OS_MODE_AUTO;
}

void os_mode_set(os_mode_t mode) {
    eeconfig_update_user(mode);
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
