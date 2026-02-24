#pragma once

#define MOTOR_RPWM 40
#define MOTOR_LPWM 41
#define MOTOR_ENABLE 42

#define MOTOR_PWM_CHANNEL 0
#define MOTOR_PWM_FREQ 20000
#define MOTOR_PWM_RESOLUTION 10

#include <GyverMotor2.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

extern GMotor2<DRIVER3WIRE, MOTOR_PWM_RESOLUTION> motor;

void motor_init();
