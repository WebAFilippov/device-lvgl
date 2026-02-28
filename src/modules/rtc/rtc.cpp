#include "rtc.h"
#include "modules/ntp/ntp.h"
#include "modules/motor/motor.h"

static RTC_PCF8563 rtc;

PCF8563_RTC rtc_module;

void PCF8563_RTC::setUnix(uint32_t unix)
{
    rtc.adjust(DateTime(unix));
}

uint32_t PCF8563_RTC::getUnix()
{
    return rtc.now().unixtime();
}

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
    if (NTP.online())
    {
        snprintf(buf, len, "%02d:%02d", NTP.hour(), NTP.minute());
    }
    else
    {
        DateTime now = rtc.now();
        now = DateTime(now.unixtime() + 7 * 3600);
        snprintf(buf, len, "%02d:%02d", now.hour(), now.minute());
    }
    return buf;
}

void clock_timer_cb(lv_timer_t *timer)
{
    (void)timer;
    char time_str[6];
    rtc_get_time_str(time_str, sizeof(time_str));

    lv_label_set_text(ui_TimeTitle, time_str);
    lv_label_set_text(ui_TimeTitle2, time_str);
}
