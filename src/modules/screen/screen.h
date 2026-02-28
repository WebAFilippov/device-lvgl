#pragma once

#include <lvgl.h>

extern lv_obj_t* current_screen;
extern bool screen_switch_requested;

void screen_tick();
