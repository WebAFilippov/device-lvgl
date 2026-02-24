#include "button.h"
#include "modules/motor/motor.h"

Button btn_theme;
Button btn_motor_up;
Button btn_motor_down;

void buttonTask(void *pvParameters)
{
    while (1)
    {
        btn_theme.tick();
        btn_motor_up.tick();
        btn_motor_down.tick();

        if (btn_motor_up.press() && !btn_motor_down.pressing() && motor.getState() == 0)
        {
            Serial0.println("MOTOR: step UP");
            motor.setSpeedPerc(100);
        }
        if (btn_motor_down.press() && !btn_motor_up.pressing() && motor.getState() == 0)
        {
            Serial0.println("MOTOR: step DOWN");
            motor.setSpeedPerc(-100);
        }

        if (btn_motor_up.release() || btn_motor_down.release())
        {
            Serial0.println("MOTOR: release step");
            motor.stop();
        }

        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

void button_init()
{
    btn_theme.init(BUTTON_THEME_PIN, INPUT_PULLUP, LOW);
    btn_motor_up.init(BUTTON_MOTOR_UP, INPUT_PULLUP, LOW);
    btn_motor_down.init(BUTTON_MOTOR_DOWN, INPUT_PULLUP, LOW);

    xTaskCreatePinnedToCore(
        buttonTask,
        "buttonTask",
        1024 * 2,
        NULL,
        2,
        NULL,
        0);

    Serial0.println("Buttons initialized");
}
