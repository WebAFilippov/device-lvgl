#ifndef MODULES_RTC_H
#define MODULES_RTC_H

#include <stdint.h>
#include <RTClib.h>
#include <Wire.h>
#include <lvgl.h>
#include "ui/ui.h"
#include "modules/ntp/ntp.h"
#include <VirtualRTC.h>

#define RTC_SDA 6
#define RTC_SCL 7

class PCF8563_RTC : public VirtualRTC {
public:
    void setUnix(uint32_t unix) override;
    uint32_t getUnix() override;
};

void rtc_init();
void rtc_set_compile_time();
char* rtc_get_time_str(char* buf, size_t len);
void clock_timer_cb(lv_timer_t* timer);

extern PCF8563_RTC rtc_module;

#endif
