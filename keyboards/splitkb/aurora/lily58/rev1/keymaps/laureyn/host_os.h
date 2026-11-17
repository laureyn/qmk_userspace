// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#pragma once

#include QMK_KEYBOARD_H
#include "os_detection.h"

// The OS the keyboard types for: OS_WINDOWS or OS_LINUX.
os_variant_t current_os(void);
