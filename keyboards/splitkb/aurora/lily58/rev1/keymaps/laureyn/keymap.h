// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#pragma once
#include QMK_KEYBOARD_H
#include "bepo_table.h"
#include "bepo_dead.h"
#include "bepo_exception.h"
#include "os_shortcuts.h"
#include "shift_key.h"
#include "host_locks.h"
#include "layers.h"
#include "settings_keys.h"

enum layers {
    BASE,
    SYM,
    NUM,
    MEDIA,
};
