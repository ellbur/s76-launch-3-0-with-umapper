// Custom process_action() override for key remapping

#include "action.h"
#include "action_code.h"
#include "eeconfig.h"
#include "umapper/main.h"
#include "process_action_remap.h"

// Set in the user EEPROM config when umapper is turned off, so that a reset
// EEPROM (all zeros) leaves umapper on
#define UMAPPER_DISABLED_BIT 0x01

static bool umapper_initialized = false;
static bool umapper_enabled = true;

static void ensure_initialized(void) {
    if (!umapper_initialized) {
        umapper_init();
        umapper_enabled = !(eeconfig_read_user() & UMAPPER_DISABLED_BIT);
        umapper_initialized = true;
    }
}

bool umapper_is_enabled(void) {
    ensure_initialized();
    return umapper_enabled;
}

void umapper_set_enabled(bool enabled) {
    ensure_initialized();
    if (enabled == umapper_enabled) {
        return;
    }

    // Release everything so keys held across the switch don't get stuck
    // (their release would be handled differently from their press)
    clear_keyboard();
    umapper_init();
    umapper_enabled = enabled;

    uint32_t user_config = eeconfig_read_user();
    if (enabled) {
        user_config &= ~UMAPPER_DISABLED_BIT;
    } else {
        user_config |= UMAPPER_DISABLED_BIT;
    }
    eeconfig_update_user(user_config);
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
    ensure_initialized();

    // Let QMK special actions through unchanged, and everything when umapper is off
    if (!umapper_enabled || !is_remappable_action(action)) {
        process_action_default(record, action);
        return;
    }

    bool pressed = record->event.pressed;
    uint16_t keycode = action.key.code;

    umapper_process_key(keycode, pressed);
}
