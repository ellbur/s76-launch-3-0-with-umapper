// Custom process_action() override for key remapping

#include "action.h"
#include "action_code.h"
#include "umapper/main.h"

static bool umapper_initialized = false;

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
    if (!umapper_initialized) {
        umapper_init();
        umapper_initialized = true;
    }

    // Let QMK special actions through unchanged
    if (!is_remappable_action(action)) {
        process_action_default(record, action);
        return;
    }

    bool pressed = record->event.pressed;
    uint16_t keycode = action.key.code;

    umapper_process_key(keycode, pressed);
}
