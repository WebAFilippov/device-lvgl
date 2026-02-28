#include "wifi.h"
#include <WiFi.h>
#include <WiFiManager.h>

static WiFiManager wm;
static bool wifi_connected = false;
static unsigned long last_check_ms = 0;
static const unsigned long CHECK_INTERVAL_MS = 5000;

void wifi_init() {
    wm.setDebugOutput(true);
    wm.setConfigPortalTimeout(180);
    wm.setMinimumSignalQuality();
    
    wm.setAPCallback([](WiFiManager *myWM) {
        Serial0.println("WiFi Config Portal started");
        Serial0.print("Connect to: ");
        Serial0.println(myWM->getConfigPortalSSID());
        Serial0.print("IP: ");
        Serial0.println(WiFi.softAPIP());
    });
    
    wm.setSaveConfigCallback([]() {
        Serial0.println("WiFi config saved, connecting...");
    });
    
    bool res = wm.autoConnect("ESP32-Setup", "setup123");
    
    if (res) {
        wifi_connected = true;
        Serial0.println("WiFi connected!");
        Serial0.print("SSID: ");
        Serial0.println(WiFi.SSID());
        Serial0.print("IP: ");
        Serial0.println(WiFi.localIP());
    } else {
        wifi_connected = false;
        Serial0.println("WiFi Config Portal running");
    }
}

void wifi_tick() {
    unsigned long now = millis();
    
    if (now - last_check_ms < CHECK_INTERVAL_MS) {
        return;
    }
    last_check_ms = now;
    
    bool connected = (WiFi.status() == WL_CONNECTED);
    
    if (connected != wifi_connected) {
        wifi_connected = connected;
        
        if (connected) {
            Serial0.println("WiFi reconnected!");
            Serial0.print("IP: ");
            Serial0.println(WiFi.localIP());
        } else {
            Serial0.println("WiFi disconnected, will retry...");
        }
    }
    
    if (!connected) {
        if (WiFi.status() == WL_IDLE_STATUS || WiFi.status() == WL_DISCONNECTED) {
            Serial0.println("Attempting WiFi reconnect...");
            WiFi.reconnect();
        }
    }
}

bool wifi_is_connected() {
    return wifi_connected && (WiFi.status() == WL_CONNECTED);
}

const char* wifi_get_ssid() {
    if (!wifi_is_connected()) return "";
    return WiFi.SSID().c_str();
}

const char* wifi_get_ip() {
    if (!wifi_is_connected()) return "0.0.0.0";
    return WiFi.localIP().toString().c_str();
}

int32_t wifi_get_rssi() {
    if (!wifi_is_connected()) return 0;
    return WiFi.RSSI();
}

void wifi_reset_settings() {
    Serial0.println("Resetting WiFi settings...");
    wm.resetSettings();
    delay(1000);
    ESP.restart();
}
