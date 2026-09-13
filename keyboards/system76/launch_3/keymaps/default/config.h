#pragma once

// Take the keymap from keymap.c and the LED settings from below on every boot,
// ignoring anything saved by the System76 Keyboard Configurator
#define LAUNCH_3_HARDCODED_CONFIG

#ifdef RGB_MATRIX_ENABLE
#    undef RGB_MATRIX_DEFAULT_MODE
#    define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_COLOR
#    undef RGB_MATRIX_DEFAULT_HUE
#    define RGB_MATRIX_DEFAULT_HUE 7
#    undef RGB_MATRIX_DEFAULT_SAT
#    define RGB_MATRIX_DEFAULT_SAT 142
#    undef RGB_MATRIX_DEFAULT_VAL
#    define RGB_MATRIX_DEFAULT_VAL 126
#    undef RGB_MATRIX_DEFAULT_SPD
#    define RGB_MATRIX_DEFAULT_SPD 125
#endif  // RGB_MATRIX_ENABLE
