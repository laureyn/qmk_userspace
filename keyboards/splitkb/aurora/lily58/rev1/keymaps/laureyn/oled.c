// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#include "oled.h"
#include "bepo_table.h"
#include "stats.h"
#include "sync.h"

static bool wanted = true;

bool oled_wanted(void) {
    return wanted;
}

void oled_set_wanted(bool on) {
    if (on == wanted) {
        return;
    }
    wanted = on;
    if (on) {
        oled_on();
    } else {
        oled_off();
    }
}

bool oled_process_record(uint16_t keycode, keyrecord_t *record) {
    if (keycode == OLED_TOG) {
        if (record->event.pressed) {
            oled_set_wanted(!wanted);
        }
        return false;
    }
    return true;
}

// Writes n as digits at p and returns the position after them.
static char *put_uint(char *p, uint32_t n) {
    char digits[10];
    uint8_t count = 0;
    do {
        digits[count++] = '0' + n % 10;
        n /= 10;
    } while (n);
    while (count) {
        *p++ = digits[--count];
    }
    return p;
}

// Fits 5 characters: 1234, 12.3k, 123k, 999k+.
static void format_count(uint32_t n, char *out) {
    char *p = out;
    if (n < 10000) {
        p = put_uint(p, n);
    } else if (n < 100000) {
        p = put_uint(p, n / 1000);
        *p++ = '.';
        p = put_uint(p, (n % 1000) / 100);
        *p++ = 'k';
    } else if (n < 1000000) {
        p = put_uint(p, n / 1000);
        *p++ = 'k';
    } else {
        p = put_uint(p, 999);
        *p++ = 'k';
        *p++ = '+';
    }
    *p = '\0';
}

// Left screen: name, and CAPS under it while Caps Word is on (DEV-101).
// Right screen: typing speed and counts (DEV-35, DEV-103, DEV-104).
// The board calls this first; returning false skips its logo and status drawing.
bool oled_task_user(void) {
    if (is_keyboard_left()) {
        oled_write_ln_P(PSTR("LEFT"), false);
        oled_write_ln_P(caps_word_shown() ? PSTR("CAPS") : PSTR("    "), false);
    } else {
        const stats_t *stats = stats_shown();
        char           text[8];

        oled_write_ln_P(PSTR("RIGHT"), false);
        oled_write_ln_P(PSTR("CPM"), false);
        format_count((uint32_t)get_current_wpm() * 5, text); // WPM x 5 = characters per minute
        oled_write_ln(text, false);
        oled_write_ln_P(PSTR("CHARS"), false);
        format_count(stats->chars, text);
        oled_write_ln(text, false);
        oled_write_ln_P(PSTR("BKSP"), false);
        format_count(stats->backspaces, text);
        oled_write_ln(text, false);
    }
    return false;
}
