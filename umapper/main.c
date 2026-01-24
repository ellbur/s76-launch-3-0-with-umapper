
#include "main.h"

#include "layout-definitions.h"
#include "mapping.h"
#include "action.h"
#include "action_code.h"
#include "timer.h"

struct state state = { };

static void init_state(void) {
  for (uint8_t i=0; i<MAX_KEYS; i++) {
    state.input_pressed_keys[i] = 0;
  }
  state.pressed_modifier_mask = 0;
  state.num_pressed_modifiers = 0;
  for (uint8_t i=0; i<MAX_MODIFIERS; i++) {
    state.pressed_modifiers[i].input_key = 0;
    state.pressed_modifiers[i].modifier_mask = 0;
  }
  state.output_modifier_mask = 0;
  state.num_pressed_output_modifiers = 0;
  for (uint8_t i=0; i<MAX_MODIFIERS; i++) {
    state.pressed_output_modifiers[i].input_key = 0;
    state.pressed_output_modifiers[i].output_modifier_mask = 0;
  }
  state.has_pressed_action_key = false;
  state.pressed_action_key.trigger = 0;
  state.pressed_action_key.to_modifiers = 0;
  state.pressed_action_key.to_action = 0;
}

static void output_event_routine(void *data, enum event_type t, key_code k) {
  action_t action;
  action.code = ACTION_KEY(k);

  keyrecord_t record = {
    .event = {
      .pressed = (t == PRESSED),
      .time = timer_read(),
      .type = KEY_EVENT
    }
#ifndef NO_ACTION_TAPPING
    , .tap = {.count = 0}
#endif
  };

  process_action_default(&record, action);
}

void umapper_init(void) {
  init_state();
}

void umapper_process_key(uint16_t keycode, bool pressed) {
  struct layout our_layout = {
    .mappings = our_mappings,
    .key_definitions = our_key_definitions,
    .num_keys = our_num_keys
  };

  if (keycode != 0 && keycode < our_num_keys) {
    step(&our_layout, &state, pressed ? PRESSED : RELEASED, (key_code)keycode, output_event_routine, 0);
  }
}

