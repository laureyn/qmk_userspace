// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#include "oled.h"
#include "keymap.h"
#include "bepo_table.h"
#include "oled_art.h"
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
    char    digits[10];
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
        p    = put_uint(p, n / 1000);
        *p++ = '.';
        p    = put_uint(p, (n % 1000) / 100);
        *p++ = 'k';
    } else if (n < 1000000) {
        p    = put_uint(p, n / 1000);
        *p++ = 'k';
    } else {
        p    = put_uint(p, 999);
        *p++ = 'k';
        *p++ = '+';
    }
    *p = '\0';
}

static const char *layer_name(uint8_t layer) {
    switch (layer) {
        case SYM:
            return "SYM";
        case NUM:
            return "NUM";
        case MEDIA:
            return "MEDIA";
        default:
            return "BASE";
    }
}

// Big letters: 5 x 7 pixels, each pixel drawn 3 x 3, so a letter is 15 x 21 pixels (3 text rows).
#define BIG_SCALE 3

static const uint8_t big_L[7] = {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F};
static const uint8_t big_R[7] = {0x1E, 0x11, 0x11, 0x1E, 0x14, 0x12, 0x11};

// Bit 4 is the left pixel of a row.
static void draw_big(const uint8_t *letter, uint8_t x0, uint8_t y0) {
    for (uint8_t row = 0; row < 7; row++) {
        for (uint8_t col = 0; col < 5; col++) {
            if (!(letter[row] & (0x10 >> col))) {
                continue;
            }
            for (uint8_t dy = 0; dy < BIG_SCALE; dy++) {
                for (uint8_t dx = 0; dx < BIG_SCALE; dx++) {
                    oled_write_pixel(x0 + col * BIG_SCALE + dx, y0 + row * BIG_SCALE + dy, true);
                }
            }
        }
    }
}

// One 24 x 24 logo: three text rows, starting at text column col.
static void draw_logo(const char *logo, uint8_t col, uint8_t line) {
    for (uint8_t page = 0; page < 3; page++) {
        oled_set_cursor(col, line + page);
        oled_write_raw_P(logo + page * 24, 24);
    }
}

// Layer stack, like a tilted floor plan. The current layer is filled, the others only outlined.
#define TILE_W 20
#define TILE_H 5
#define TILE_SKEW 3  // the top row starts this many pixels right of the bottom row
#define TILE_PITCH 6 // pixels from one tile to the next
#define STACK_Y 24

static void draw_tile(uint8_t x0, uint8_t y0, bool filled) {
    for (uint8_t row = 0; row < TILE_H; row++) {
        uint8_t shift = (TILE_H - 1 - row) * TILE_SKEW / (TILE_H - 1);
        for (uint8_t col = shift; col < shift + TILE_W; col++) {
            bool edge = row == 0 || row == TILE_H - 1 || col == shift || col == shift + TILE_W - 1;
            if (filled || edge) {
                oled_write_pixel(x0 + col, y0 + row, true);
            }
        }
    }
}

static void draw_layer_stack(uint8_t current) {
    // BASE sits at the bottom, MEDIA at the top.
    for (uint8_t layer = BASE; layer <= MEDIA; layer++) {
        draw_tile(0, STACK_Y + (MEDIA - layer) * TILE_PITCH, layer == current);
    }
}

// Left screen: a big L, the OS logo, the layer stack with the layer name below it,
// and LOCK, CAPS and SHIFT (inverted when Shift is locked) on the right.
static void draw_left(status_t status) {
    draw_big(big_L, 0, 0);
    draw_logo(status.os == OS_LINUX ? tux_logo : windows_logo, 17, 0);
    draw_layer_stack(status.layer);

    oled_set_cursor(0, 6);
    oled_write(layer_name(status.layer), false);

    if (status.layer_locked) {
        oled_set_cursor(11, 3);
        oled_write_P(PSTR("LOCK"), true);
    }
    if (status.caps_word) {
        oled_set_cursor(11, 4);
        oled_write_P(PSTR("CAPS"), false);
    }
    if (status.shift_locked) {
        oled_set_cursor(11, 5);
        oled_write_P(PSTR("SHIFT"), true);
    }
}

// Right screen: a big R, then typing speed and counts beside it (DEV-35, DEV-103, DEV-104).
static void draw_right(const stats_t *stats) {
    char text[8];

    draw_big(big_R, 0, 0);

    oled_set_cursor(4, 0);
    oled_write_P(PSTR("CPM"), false);
    oled_set_cursor(4, 1);
    format_count((uint32_t)get_current_wpm() * 5, text); // WPM x 5 = characters per minute
    oled_write(text, false);

    oled_set_cursor(4, 2);
    oled_write_P(PSTR("CHARS"), false);
    oled_set_cursor(4, 3);
    format_count(stats->chars, text);
    oled_write(text, false);

    oled_set_cursor(4, 4);
    oled_write_P(PSTR("BKSP"), false);
    oled_set_cursor(4, 5);
    format_count(stats->backspaces, text);
    oled_write(text, false);
}

// The board calls this first; returning false skips its logo and status drawing.
bool oled_task_user(void) {
    oled_clear();
    if (is_keyboard_left()) {
        draw_left(status_shown());
    } else {
        draw_right(stats_shown());
    }
    return false;
}
