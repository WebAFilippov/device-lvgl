#include "rtc.h"

RTC_PCF8563 rtc;

void rtc_init()
{
    Wire.begin(RTC_SDA, RTC_SCL);

    if (!rtc.begin())
    {
        Serial0.println("Couldn't find RTC!");
        while (1)
            delay(10);
    }

    if (rtc.lostPower())
    {
        Serial0.println("RTC lost power, setting default time!");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    rtc.start();
}

void rtc_set_compile_time()
{
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

char *rtc_get_time_str(char *buf, size_t len)
{
    DateTime now = rtc.now();
    snprintf(buf, len, "%02d:%02d", now.hour(), now.minute());
    return buf;
}

void clock_timer_cb(lv_timer_t *timer)
{
    (void)timer;
    char time_str[6];
    rtc_get_time_str(time_str, sizeof(time_str));
    lv_label_set_text(ui_TimeTitle, time_str);
}