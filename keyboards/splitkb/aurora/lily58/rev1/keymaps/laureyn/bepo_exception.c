// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#include "bepo_exception.h"

// The hex digits of the Unicode characters, as keycodes.
// Belgian digits need Shift, and the keycodes already carry it.
static const uint16_t cedilla_hex[] = {BE_C, BE_7};              // Ç  U+00C7
static const uint16_t ellipsis_hex[] = {BE_2, BE_0, BE_2, BE_6}; // …  U+2026

// For the Ç key: the keycode held while its tap is down (0 = none).
static uint16_t held_cced = 0;

// Linux: Ctrl+Shift+U, the hex digits, then Space. Ctrl and Shift from the user are
// taken out for the sequence, so the digits are not sent with them.
static void send_unicode_linux(const uint16_t *hex, uint8_t count) {
    uint8_t saved_mods = get_mods();
    clear_mods();

    tap_code16(C(S(BE_U)));
    for (uint8_t i = 0; i < count; i++) {
        tap_code16(hex[i]);
    }
    tap_code16(KC_SPC);

    set_mods(saved_mods);
}

bool bepo_exception_process_record(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case BP_ELLIPSIS:
            if (record->event.pressed) {
                send_unicode_linux(ellipsis_hex, sizeof(ellipsis_hex) / sizeof(ellipsis_hex[0]));
            }
            return false;

        case BP_CCED:
            if (record->event.pressed) {
                if (get_mods() & MOD_MASK_SHIFT) {
                    send_unicode_linux(cedilla_hex, sizeof(cedilla_hex) / sizeof(cedilla_hex[0]));
                } else {
                    register_code16(BE_CCED);
                    held_cced = BE_CCED;
                }
            } else if (held_cced) {
                unregister_code16(held_cced);
                held_cced = 0;
            }
            return false;

        default:
            return true;
    }
}
