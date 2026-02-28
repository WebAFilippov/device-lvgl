#pragma once

#include <EncButton.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define EB_DEB_TIME 50    // таймаут гашения дребезга кнопки (кнопка)
#define EB_CLICK_TIME 500 // таймаут ожидания кликов (кнопка)
#define EB_HOLD_TIME 600  // таймаут удержания (кнопка)
#define EB_STEP_TIME 200  // таймаут импульсного удержания (кнопка)
#define EB_FAST_TIME 30   // таймаут быстрого поворота (энкодер)
#define EB_TOUT_TIME 1000 // таймаут действия (кнопка и энкодер)

#define BUTTON_CHANGE_SCREEN 15
#define BUTTON_MOTOR_UP 4
#define BUTTON_MOTOR_DOWN 5

extern Button btn_change_screen;
extern Button btn_motor_up;
extern Button btn_motor_down;

void button_init();
