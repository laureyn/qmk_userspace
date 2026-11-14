#!/usr/bin/env bash
# Draws the keymap picture into docs/keymap.svg. Run from the repo root.
# Needs: qmk (CLI) and keymap-drawer on PATH, QMK_HOME pointing at qmk_firmware.
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

KB="splitkb/aurora/lily58/rev1"
KEYMAP_C="${ROOT}/keyboards/${KB}/keymaps/laureyn/keymap.c"
OUT_DIR="${ROOT}/docs"
WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT

mkdir -p "$OUT_DIR"
qmk c2json -kb "$KB" -km laureyn "$KEYMAP_C" > "$WORK/keymap.json"
keymap parse -q "$WORK/keymap.json" -o "$WORK/keymap.yaml"
keymap draw "$WORK/keymap.yaml" -o "$OUT_DIR/keymap.svg"
echo "Wrote $OUT_DIR/keymap.svg"
