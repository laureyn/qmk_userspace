// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#include "oled.h"
#include "keymap.h"
#include "bepo_table.h"
#include "oled_art.h"
#include "stats.h"
#include "sync.h"

// The panel is 128 x 64 and is rotated 270 degrees, so the screen is 64 wide and 128 tall.
// Everything is placed by pixel, so it can be centred.
#define SCREEN_W 64
#define SCREEN_H 128

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

// Writes n / unit with one decimal, without a trailing ".0": 1.5, 9.9, 12.
static char *put_decimal(char *p, uint32_t n, uint32_t unit) {
    p             = put_uint(p, n / unit);
    uint8_t tenth = (n % unit) / (unit / 10);
    if (tenth) {
        *p++ = '.';
        *p++ = '0' + tenth;
    }
    return p;
}

// Human-readable: 999, 1K, 1.5K, 12K, 999K, 1.2M, 12M.
static void format_count(uint32_t n, char *out) {
    char *p = out;
    if (n < 1000) {
        p = put_uint(p, n);
    } else if (n < 10000) {
        p    = put_decimal(p, n, 1000);
        *p++ = 'K';
    } else if (n < 1000000) {
        p    = put_uint(p, n / 1000);
        *p++ = 'K';
    } else if (n < 10000000) {
        p    = put_decimal(p, n, 1000000);
        *p++ = 'M';
    } else {
        p    = put_uint(p, n / 1000000);
        *p++ = 'M';
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

// 5 x 7 font for the letters and digits on the screens. Each row is 5 pixels, leftmost in bit 4.
typedef struct {
    char    c;
    uint8_t rows[7];
} glyph_t;

static const glyph_t glyphs[] = {
    {'.', {0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C}}, {'0', {0x0E, 0x11, 0x13, 0x15, 0x19, 0x11, 0x0E}}, {'1', {0x04, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x0E}}, {'2', {0x0E, 0x11, 0x01, 0x02, 0x04, 0x08, 0x1F}}, {'3', {0x1E, 0x01, 0x01, 0x0E, 0x01, 0x01, 0x1E}}, {'4', {0x02, 0x06, 0x0A, 0x12, 0x1F, 0x02, 0x02}}, {'5', {0x1F, 0x10, 0x1E, 0x01, 0x01, 0x11, 0x0E}}, {'6', {0x06, 0x08, 0x10, 0x1E, 0x11, 0x11, 0x0E}}, {'7', {0x1F, 0x01, 0x02, 0x04, 0x08, 0x08, 0x08}}, {'8', {0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E}}, {'9', {0x0E, 0x11, 0x11, 0x0F, 0x01, 0x02, 0x0C}}, {'A', {0x0E, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11}}, {'B', {0x1E, 0x11, 0x11, 0x1E, 0x11, 0x11, 0x1E}}, {'C', {0x0E, 0x11, 0x10, 0x10, 0x10, 0x11, 0x0E}}, {'D', {0x1C, 0x12, 0x11, 0x11, 0x11, 0x12, 0x1C}},
    {'E', {0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F}}, {'F', {0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x10}}, {'H', {0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11}}, {'I', {0x0E, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0E}}, {'K', {0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11}}, {'L', {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F}}, {'M', {0x11, 0x1B, 0x15, 0x15, 0x11, 0x11, 0x11}}, {'N', {0x11, 0x19, 0x15, 0x13, 0x11, 0x11, 0x11}}, {'O', {0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E}}, {'P', {0x1E, 0x11, 0x11, 0x1E, 0x10, 0x10, 0x10}}, {'R', {0x1E, 0x11, 0x11, 0x1E, 0x14, 0x12, 0x11}}, {'S', {0x0F, 0x10, 0x10, 0x0E, 0x01, 0x01, 0x1E}}, {'T', {0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04}}, {'U', {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E}}, {'Y', {0x11, 0x11, 0x0A, 0x04, 0x04, 0x04, 0x04}},
};

static const uint8_t *glyph_rows(char c) {
    for (uint8_t i = 0; i < sizeof(glyphs) / sizeof(glyphs[0]); i++) {
        if (glyphs[i].c == c) {
            return glyphs[i].rows;
        }
    }
    return NULL; // unknown characters are blank
}

// Width in pixels of text at a scale: 6 pixels per letter, the last gap left out.
static uint8_t text_width(const char *s, uint8_t scale) {
    uint8_t n = 0;
    while (s[n]) {
        n++;
    }
    return n ? n * 6 * scale - scale : 0;
}

static void draw_text(const char *s, uint8_t x, uint8_t y, uint8_t scale) {
    for (; *s; s++) {
        const uint8_t *rows = glyph_rows(*s);
        if (rows) {
            for (uint8_t row = 0; row < 7; row++) {
                for (uint8_t col = 0; col < 5; col++) {
                    if (!(rows[row] & (0x10 >> col))) {
                        continue;
                    }
                    for (uint8_t dy = 0; dy < scale; dy++) {
                        for (uint8_t dx = 0; dx < scale; dx++) {
                            oled_write_pixel(x + (col * scale) + dx, y + (row * scale) + dy, true);
                        }
                    }
                }
            }
        }
        x += 6 * scale;
    }
}

// Text centred horizontally on the screen.
static void draw_text_centred(const char *s, uint8_t y, uint8_t scale) {
    draw_text(s, (SCREEN_W - text_width(s, scale)) / 2, y, scale);
}

// A 24 x 24 bitmap in the format of oled_art.h, drawn at pixel x0, y0.
static void draw_logo(const char *logo, uint8_t x0, uint8_t y0) {
    for (uint8_t page = 0; page < 3; page++) {
        for (uint8_t col = 0; col < 24; col++) {
            uint8_t byte = pgm_read_byte(&logo[page * 24 + col]);
            for (uint8_t bit = 0; bit < 8; bit++) {
                if (byte & (1 << bit)) {
                    oled_write_pixel(x0 + col, y0 + (page * 8) + bit, true);
                }
            }
        }
    }
}

// Layer stack, like a tilted floor plan. The current layer is filled, the others only outlined.
#define TILE_W 20
#define TILE_H 5
#define TILE_SKEW 3  // the top row starts this many pixels right of the bottom row
#define TILE_PITCH 6 // pixels from one tile to the next
#define STACK_H (3 * TILE_PITCH + TILE_H)
#define STACK_W (TILE_W + TILE_SKEW)
#define STACK_X ((SCREEN_W - STACK_W) / 2) // horizontally centred
#define STACK_Y ((SCREEN_H - STACK_H) / 2) // vertically centred

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
        draw_tile(STACK_X, STACK_Y + (MEDIA - layer) * TILE_PITCH, layer == current);
    }
}

// Left screen, top to bottom:
//   big L, CAPS, SHIFT, layer stack (centred), layer name, LOCK, OS logo (bottom).
#define LOGO_X ((SCREEN_W - 24) / 2)
#define LOGO_Y (SCREEN_H - 24)
#define NAME_Y (STACK_Y + STACK_H + 4)

static void draw_left(status_t status) {
    draw_text_centred("L", 0, 3);

    if (status.caps_word) {
        draw_text_centred("CAPS", 22, 1);
    }
    if (status.shift_locked) {
        draw_text_centred("SHIFT", 31, 1);
    }

    draw_layer_stack(status.layer);
    draw_text_centred(layer_name(status.layer), NAME_Y, 1);

    if (status.layer_locked) {
        draw_text_centred("LOCK", LOGO_Y - 10, 1);
    }

    draw_logo(status.os == OS_LINUX ? tux_logo : windows_logo, LOGO_X, LOGO_Y);
}

// One statistic on the right screen: the label, then the large value under it, both centred.
#define METRIC_H 22 // label (7 pixels), gap, value (14 pixels)

static void draw_metric(const char *label, const char *value, uint8_t y) {
    draw_text_centred(label, y, 1);
    draw_text_centred(value, y + 8, 2);
}

// Right screen: a big R at the top, the statistics stacked at the bottom of the screen.
static void draw_right(const stats_t *stats) {
    char value[8];

    draw_text_centred("R", 0, 3);

    // The three blocks end at the bottom of the screen, with 8 pixels between them.
    uint8_t y = SCREEN_H - (3 * METRIC_H + 2 * 8);

    format_count((uint32_t)get_current_wpm() * 5, value); // WPM x 5 = characters per minute
    draw_metric("CPM", value, y);
    format_count(stats->chars, value);
    draw_metric("CHARS", value, y + METRIC_H + 8);
    format_count(stats->backspaces, value);
    draw_metric("BKSP", value, y + 2 * (METRIC_H + 8));
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
