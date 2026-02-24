#ifndef MODULES_RTC_H
#define MODULES_RTC_H

#include <RTClib.h>
#include <Wire.h>
#include <lvgl.h>
#include "ui/ui.h"

#define RTC_SDA 6
#define RTC_SCL 7

extern RTC_PCF8563 rtc;

void rtc_init();

void clock_timer_cb(lv_timer_t *timer);

#endif
