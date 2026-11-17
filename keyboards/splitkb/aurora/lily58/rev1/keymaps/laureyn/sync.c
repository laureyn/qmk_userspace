// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#include "transactions.h"
#include "sync.h"
#include "oled.h"
#include "layers.h"
#include "shift_key.h"
#include "host_os.h"

#define SYNC_INTERVAL_MS 200

static user_sync_t from_master;
static uint32_t    sync_timer = 0;

static status_t status_local(void) {
    return (status_t){
        .layer        = get_highest_layer(layer_state),
        .layer_locked = layer_top_locked(),
        .shift_locked = shift_key_locked(),
        .caps_word    = is_caps_word_on(),
        .os           = current_os(),
        .oled_enabled = oled_wanted(),
    };
}

// Runs on the half without the USB cable.
static void user_sync_slave_handler(uint8_t in_size, const void *in_data, uint8_t out_size, void *out_data) {
    const user_sync_t *in = in_data;
    from_master           = *in;
    stats_from_master(&in->stats);
    oled_set_wanted(in->status.oled_enabled);
}

void keyboard_post_init_user(void) {
    stats_load();
    transaction_register_rpc(USER_SYNC_STATE, user_sync_slave_handler);
}

status_t status_shown(void) {
    return is_keyboard_master() ? status_local() : from_master.status;
}

// Sends the state a few times per second, so a half that restarts gets it again.
void sync_task(void) {
    if (!is_keyboard_master() || timer_elapsed32(sync_timer) < SYNC_INTERVAL_MS) {
        return;
    }
    sync_timer        = timer_read32();
    user_sync_t state = {
        .status = status_local(),
        .stats  = *stats_local(),
    };
    transaction_rpc_send(USER_SYNC_STATE, sizeof(state), &state);
}
