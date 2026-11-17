// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#include "os_shortcuts.h"
#include "host_os.h"

// The OS keys on MEDIA (choose auto / Windows / Linux) and Redo, which depends on the OS.
bool os_shortcuts_process_record(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case OSKEY_AUTO:
            if (record->event.pressed) {
                os_mode_set(OS_MODE_AUTO);
            }
            return false;

        case OSKEY_WINDOWS:
            if (record->event.pressed) {
                os_mode_set(OS_MODE_WINDOWS);
            }
            return false;

        case OSKEY_LINUX:
            if (record->event.pressed) {
                os_mode_set(OS_MODE_LINUX);
            }
            return false;

        case OS_REDO:
            if (record->event.pressed) {
                // Ctrl+Y on Windows, Ctrl+Shift+Z on Linux. Our own Shift/Ctrl are taken out
                // for the shortcut and restored afterwards, as in bepo_exception.c.
                uint8_t saved_mods = get_mods();
                clear_mods();
                tap_code16(current_os() == OS_LINUX ? C(S(KC_Z)) : C(KC_Y));
                set_mods(saved_mods);
            }
            return false;

        default:
            return true;
    }
}
