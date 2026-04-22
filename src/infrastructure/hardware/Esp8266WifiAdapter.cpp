#include "Esp8266WifiAdapter.h"

bool Esp8266WifiAdapter::initialize() {
    Serial.println("Initializing WiFi connection...");
    
    setupWifiMode();
    addWifiNetworks();
    
    Serial.printf("WiFi configured for SSID: %s\n", WIFI_SSID);
    return true;
}

bool Esp8266WifiAdapter::isConnected() const {
    return wifiMulti.run() == WL_CONNECTED;
}

bool Esp8266WifiAdapter::run() {
    return wifiMulti.run() == WL_CONNECTED;
}

void Esp8266WifiAdapter::setupWifiMode() {
    WiFi.mode(WIFI_STA);
}

void Esp8266WifiAdapter::addWifiNetworks() {
    wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
} 