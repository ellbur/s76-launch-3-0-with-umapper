// Custom process_action() override for key remapping
// This file demonstrates how to remap keys based on keycodes

#include "action.h"
#include "action_code.h"
#include "timer.h"

// Helper function to emit a key with given keycode and press/release state
static void emit_key(uint16_t keycode, bool pressed) {
    action_t new_action;
    new_action.code = ACTION_KEY(keycode);

    keyrecord_t new_record = {
        .event = {
            .pressed = pressed,
            .time = timer_read(),
            .type = KEY_EVENT
        }
#ifndef NO_ACTION_TAPPING
        , .tap = {.count = 0}
#endif
    };

    process_action_default(&new_record, new_action);
}

// Check if this action type should be remapped
static bool is_remappable_action(action_t action) {
    uint8_t kind = action.kind.id;

    switch (kind) {
        case ACT_LMODS:
        case ACT_RMODS:
        case ACT_LMODS_TAP:
        case ACT_RMODS_TAP:
            return true;  // Normal keys - remap them

        case ACT_LAYER:
        case ACT_LAYER_TAP:
        case ACT_LAYER_TAP_EXT:
        case ACT_LAYER_MODS:
        case ACT_USAGE:
        case ACT_MOUSEKEY:
        case ACT_SWAP_HANDS:
            return false;  // QMK special functions - don't remap

        default:
            return false;
    }
}

// Override process_action to implement custom key remapping
void process_action(keyrecord_t *record, action_t action) {
    // Let QMK special actions through unchanged
    if (!is_remappable_action(action)) {
        process_action_default(record, action);
        return;
    }

    bool pressed = record->event.pressed;
    uint16_t keycode = action.key.code;

    // Example remapping logic - replace with your own

    // Example 1: Simple 1-to-1 remapping
    // Map KC_A to KC_B
    if (keycode == KC_A) {
        emit_key(KC_B, pressed);
        return;
    }

    // Example 2: One key produces multiple keys
    // Map KC_M to emit L+S+D chord
    if (keycode == KC_M) {
        emit_key(KC_L, pressed);
        emit_key(KC_S, pressed);
        emit_key(KC_D, pressed);
        return;
    }

    // Example 3: Use modifiers by emitting modifier keys
    // Map KC_X to Shift+Y
    if (keycode == KC_X) {
        if (pressed) {
            emit_key(KC_LSFT, true);
            emit_key(KC_Y, true);
        } else {
            emit_key(KC_Y, false);
            emit_key(KC_LSFT, false);
        }
        return;
    }

    // Example 4: Suppress a key (no output)
    if (keycode == KC_Z) {
        // Do nothing - key is suppressed
        return;
    }

    // Example 5: Invert press/release
    // (uncommon, but possible)
    if (keycode == KC_Q) {
        emit_key(KC_Q, !pressed);  // Sends release on press, press on release
        return;
    }

    // Default: pass through unchanged
    process_action_default(record, action);
}
