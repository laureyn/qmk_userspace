// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#include "os_shortcuts.h"
#include "host_os.h"

bool os_shortcuts_process_record(uint16_t keycode, keyrecord_t *record) {
    if (keycode != OS_REDO) {
        return true;
    }
    if (record->event.pressed) {
        // Ctrl+Y on Windows, Ctrl+Shift+Z on Linux. Our own Shift/Ctrl are taken out
        // for the shortcut and restored afterwards, as in bepo_exception.c.
        uint8_t saved_mods = get_mods();
        clear_mods();
        tap_code16(current_os() == OS_LINUX ? C(S(KC_Z)) : C(KC_Y));
        set_mods(saved_mods);
    }
    return false;
}
