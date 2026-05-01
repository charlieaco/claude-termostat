// test/integration/test_wifi/test_wifi.cpp
#include <Arduino.h>
#include <unity.h>
#include <ESP8266WiFi.h>
#include "Esp8266WifiAdapter.h"

static Esp8266WifiAdapter* wifi;

void setUp() {}
void tearDown() {}

void test_wifi_connects_successfully() {
    wifi = new Esp8266WifiAdapter();
    bool connected = wifi->initialize();
    TEST_ASSERT_TRUE_MESSAGE(connected, "WiFi no pudo conectar — verificar credentials.h");
    delete wifi;
}

void test_wifi_reports_connected_after_init() {
    wifi = new Esp8266WifiAdapter();
    wifi->initialize();
    TEST_ASSERT_TRUE(wifi->isConnected());
    delete wifi;
}

void test_wifi_has_valid_ip() {
    wifi = new Esp8266WifiAdapter();
    wifi->initialize();
    IPAddress ip = WiFi.localIP();
    TEST_ASSERT_FALSE_MESSAGE(
        ip == IPAddress(0, 0, 0, 0),
        "IP es 0.0.0.0 — WiFi no está conectado"
    );
    delete wifi;
}

void setup() {
    Serial.begin(115200);
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_wifi_connects_successfully);
    RUN_TEST(test_wifi_reports_connected_after_init);
    RUN_TEST(test_wifi_has_valid_ip);
    UNITY_END();
}

void loop() {}
