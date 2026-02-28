#include "ntp.h"
#include "modules/rtc/rtc.h"

void ntp_init()
{
    NTP.begin(7);
    NTP.setPeriod(3600);
    NTP.attachRTC(rtc_module);
}

void ntp_tick()
{
    NTP.tick();
}

bool ntp_online()
{
    return NTP.online();
}
