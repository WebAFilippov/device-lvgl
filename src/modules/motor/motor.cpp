#include "motor.h"

const uint8_t ADC_RES = 12;
const float EMA_ALPHA = 0.125;

uint16_t rawValue = 0;
float filteredValue = 0;

GMotor2<DRIVER3WIRE, MOTOR_PWM_RESOLUTION> motor(MOTOR_RPWM, MOTOR_LPWM, MOTOR_ENABLE, MOTOR_PWM_CHANNEL);

void motorTask(void *pvParameters)
{
    while (1)
    {
        motor.tick();

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

int8_t motor_get_state()
{
    return motor.getState();
}

int16_t motor_get_speed()
{
    return motor.getSpeed();
}

int motor_get_filtered()
{
    rawValue = analogRead(IS_PIN);
    filteredValue = filteredValue * (1 - EMA_ALPHA) + rawValue * EMA_ALPHA;

    return (int)rawValue;
}

void motor_init()
{
    analogReadResolution(ADC_RES);
    analogSetAttenuation(ADC_11db);

    motor.setMinDutyPerc(50);
    motor.setAccelPerc(75);
    motor.setDeadtime(10);

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

void motor_moved_up()
{
    motor.setSpeedPerc(100);
}

void motor_moved_down()
{
    motor.setSpeedPerc(-100);
}

void motor_brake()
{
    motor.stop();
}