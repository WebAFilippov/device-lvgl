#include "modules/rtc/rtc.h"
#include "modules/button/button.h"
#include "modules/display/display.h"
#include "modules/motor/motor.h"
#include "modules/wifi/wifi.h"
#include "ui/ui.h"
#include <lvgl.h>

void setup()
{
    Serial0.begin(115200);

    rtc_init();

    button_init();
    motor_init();
    display_init();
    wifi_init();

    Serial0.println("Setup done");
}

void loop()
{
    wifi_tick();
}
