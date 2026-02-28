#include "button.h"
#include "modules/motor/motor.h"
#include "modules/screen/screen.h"
#include "modules/display/display.h"

Button btn_change_screen;
Button btn_motor_up;
Button btn_motor_down;

void buttonTask(void *pvParameters)
{
    while (1)
    {
        btn_change_screen.tick();
        btn_motor_up.tick();
        btn_motor_down.tick();

        if (btn_change_screen.click())
        {
            screen_switch_requested = true;
        }

        if (btn_change_screen.hasClicks(2))
        {
            display_toggle_theme();
        }

        if (btn_motor_up.press() && !btn_motor_down.pressing() && motor_get_state() == 0)
        {
            motor_moved_up();
        }
        if (btn_motor_down.press() && !btn_motor_up.pressing() && motor_get_state() == 0)
        {
            motor_moved_down();
        }
        if (btn_motor_up.release() || btn_motor_down.release())
        {
            motor_brake();
        }

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

void button_init()
{
    btn_change_screen.init(BUTTON_CHANGE_SCREEN, INPUT_PULLUP, LOW);
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
