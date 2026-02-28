#ifndef MODULES_NTP_H
#define MODULES_NTP_H

#include <GyverNTP.h>

void ntp_init();
void ntp_tick();
bool ntp_online();

#endif
