
#pragma once

#include <stdint.h>
#include <stdbool.h>

void umapper_init(void);
void umapper_process_key(uint16_t keycode, bool pressed);

