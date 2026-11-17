// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#include "stats.h"
#include "bepo_table.h"

// Saved at most this often, and only when the counts changed (flash wear, NFR-7).
#define SAVE_INTERVAL_MS (10UL * 60UL * 1000UL)

// Modifier bits inside a keycode: left Ctrl, left GUI and right Ctrl/GUI.
#define CTRL_GUI_KEYCODE_BITS 0x0900

static stats_t  local;
static stats_t  remote;
static bool     unsaved    = false;
static uint32_t save_timer = 0;

// Letters, digits and punctuation; Space and Enter count as typing too.
static bool is_typed_key(uint16_t keycode) {
    if (keycode >= SAFE_RANGE) {
        return keycode >= BP_COMM && keycode <= BP_ELLIPSIS;
    }
    if (keycode & CTRL_GUI_KEYCODE_BITS) {
        return false;
    }
    uint8_t base = keycode & 0xFF;
    return (base >= KC_A && base <= KC_0) || (base >= KC_MINS && base <= KC_SLSH) || base == KC_SPC || base == KC_ENT;
}

// Position of the counts in the user data block (the OS override is at 0).
#define STATS_OFFSET 4

static void save_now(void) {
    eeconfig_update_user_datablock(&local, STATS_OFFSET, sizeof(local));
    unsaved    = false;
    save_timer = timer_read32();
}

bool stats_process_record(uint16_t keycode, keyrecord_t *record) {
    if (keycode == STATS_RESET) {
        if (record->event.pressed) {
            local = (stats_t){0, 0};
            save_now();
        }
        return false;
    }
    if (!record->event.pressed) {
        return true;
    }
    if (keycode == KC_BSPC) {
        local.backspaces++;
        unsaved = true;
    } else if (is_typed_key(keycode) && !(get_mods() & (MOD_MASK_CTRL | MOD_MASK_GUI))) {
        local.chars++;
        unsaved = true;
    }
    return true;
}

void stats_load(void) {
    if (eeconfig_is_user_datablock_valid()) {
        eeconfig_read_user_datablock(&local, STATS_OFFSET, sizeof(local));
    }
}

void stats_task(void) {
    if (!is_keyboard_master() || !unsaved || timer_elapsed32(save_timer) < SAVE_INTERVAL_MS) {
        return;
    }
    save_now();
}

const stats_t *stats_local(void) {
    return &local;
}

const stats_t *stats_shown(void) {
    return is_keyboard_master() ? &local : &remote;
}

void stats_from_master(const stats_t *stats) {
    remote = *stats;
}
