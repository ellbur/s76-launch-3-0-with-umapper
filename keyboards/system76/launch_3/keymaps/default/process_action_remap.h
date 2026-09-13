#pragma once

#include <stdbool.h>

// Turn umapper remapping on or off (saved to EEPROM)
void umapper_set_enabled(bool enabled);
bool umapper_is_enabled(void);
