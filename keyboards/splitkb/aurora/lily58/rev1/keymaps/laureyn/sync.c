// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#include "transactions.h"
#include "sync.h"
#include "oled.h"

#define SYNC_INTERVAL_MS 200

static bool     caps_word_from_master = false;
static uint32_t sync_timer            = 0;

// Runs on the half without the USB cable.
static void user_sync_slave_handler(uint8_t in_size, const void *in_data, uint8_t out_size, void *out_data) {
    const user_sync_t *in = in_data;
    caps_word_from_master = in->caps_word;
    stats_from_master(&in->stats);
    oled_set_wanted(in->oled_enabled);
}

void keyboard_post_init_user(void) {
    stats_load();
    transaction_register_rpc(USER_SYNC_STATE, user_sync_slave_handler);
}

bool caps_word_shown(void) {
    return is_keyboard_master() ? is_caps_word_on() : caps_word_from_master;
}

// Sends the state a few times per second, so a half that restarts gets it again.
void sync_task(void) {
    if (!is_keyboard_master() || timer_elapsed32(sync_timer) < SYNC_INTERVAL_MS) {
        return;
    }
    sync_timer        = timer_read32();
    user_sync_t state = {
        .caps_word    = is_caps_word_on(),
        .oled_enabled = oled_wanted(),
        .stats        = *stats_local(),
    };
    transaction_rpc_send(USER_SYNC_STATE, sizeof(state), &state);
}
