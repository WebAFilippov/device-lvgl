#ifndef MODULES_DISPLAY_H
#define MODULES_DISPLAY_H

#include <stdint.h>

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

void display_init();
uint8_t display_get_theme();
void display_set_theme(uint8_t theme);
void display_toggle_theme();

#endif
