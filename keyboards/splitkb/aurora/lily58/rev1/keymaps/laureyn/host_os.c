// Copyright 2026 laureyn (@laureyn)
// SPDX-License-Identifier: GPL-2.0-only

#include "host_os.h"

// Only Windows and Linux are supported (D-03, D-16). Anything else the keyboard
// cannot tell apart (still unsure, macOS, iOS) is treated as Windows.
os_variant_t current_os(void) {
    return detected_host_os() == OS_LINUX ? OS_LINUX : OS_WINDOWS;
}
