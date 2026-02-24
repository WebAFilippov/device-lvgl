#ifndef MODULES_WIFI_H
#define MODULES_WIFI_H

#include <stdbool.h>
#include <stdint.h>

#define WIFI_RESET_BUTTON_HOLD_MS 5000

void wifi_init();
void wifi_tick();
bool wifi_is_connected();
const char* wifi_get_ssid();
const char* wifi_get_ip();
int32_t wifi_get_rssi();
void wifi_reset_settings();

#endif
