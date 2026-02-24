#include "motor.h"

GMotor2<DRIVER3WIRE, MOTOR_PWM_RESOLUTION> motor(MOTOR_RPWM, MOTOR_LPWM, MOTOR_ENABLE);

void motorTask(void *pvParameters)
{
    while (1)
    {
        motor.tick();

        Serial0.print("Motor speed: ");
        Serial0.println(motor.getSpeed());

        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

void motor_init()
{
    motor.setMinDutyPerc(50);    // старт без залипания
    motor.setAccelPerc(80);    // плавно
    motor.setDeadtime(10);     // защита H-моста

    xTaskCreatePinnedToCore(
        motorTask,
        "motorTask",
        1024 * 2,
        NULL,
        2,
        NULL,
        0);

    Serial0.println("Motor initialized");
}
