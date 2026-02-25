#include "motor.h"

const uint8_t IS_PIN = 17;     // ADC пин ESP32-S3, куда подключен делитель
const uint8_t ADC_RES = 12;    // разрешение ADC (ESP32-S3)
const float EMA_ALPHA = 0.125; // коэффициент фильтра (EMA)

uint16_t rawValue = 0;
float filteredValue = 0;

GMotor2<DRIVER3WIRE, MOTOR_PWM_RESOLUTION> motor(MOTOR_RPWM, MOTOR_LPWM, MOTOR_ENABLE, MOTOR_PWM_CHANNEL);

void motorTask(void *pvParameters)
{
    while (1)
    {
        motor.tick();

        rawValue = analogRead(IS_PIN);

        // EMA фильтр
        filteredValue = filteredValue * (1 - EMA_ALPHA) + rawValue * EMA_ALPHA;

        // Определяем триггеры
        const char *status = "UNKNOWN";

        if (filteredValue < 50)
            status = "IDLE"; // простой холостой ход
        else if (filteredValue >= 800 && filteredValue <= 900)
            status = "MOVING DOWN"; // движение вниз
        else if (filteredValue >= 1100 && filteredValue <= 1200)
            status = "MOVING UP"; // движение вверх
        else if (filteredValue > 1500)
            status = "WARNING: HIGH CURRENT"; // превышение безопасного тока
        else
            status = "NORMAL";

        // Выводим в Serial
        Serial0.print("RAW ADC: ");
        Serial0.print(rawValue);
        Serial0.print(" | Filtered: ");
        Serial0.print((int)filteredValue);
        Serial0.print(" | Status: ");
        Serial0.println(status);

        vTaskDelay(pdMS_TO_TICKS(50)); // обновление каждые 50 мс
    }
}

void motor_init()
{
    analogReadResolution(ADC_RES);  // 12 бит, 0-4095
    analogSetAttenuation(ADC_11db); // до ~3.6 В

    motor.setMinDutyPerc(50); // старт без залипания
    motor.setAccelPerc(80);   // плавно
    motor.setDeadtime(10);    // защита H-моста

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
