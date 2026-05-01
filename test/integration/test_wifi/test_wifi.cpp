// test/integration/test_wifi/test_wifi.cpp
#include <Arduino.h>
#include <unity.h>
#include <ESP8266WiFi.h>
#include "Esp8266WifiAdapter.h"

static Esp8266WifiAdapter* wifi;

void setUp() {
    wifi = new Esp8266WifiAdapter();
    wifi->initialize();
}

void tearDown() {
    delete wifi;
    wifi = nullptr;
}

void test_wifi_connects_successfully() {
    TEST_ASSERT_TRUE_MESSAGE(wifi->isConnected(), "WiFi no pudo conectar — verificar credentials.h");
}

void test_wifi_has_valid_ip() {
    IPAddress ip = WiFi.localIP();
    TEST_ASSERT_FALSE_MESSAGE(
        ip == IPAddress(0, 0, 0, 0),
        "IP es 0.0.0.0 — WiFi no esta conectado"
    );
}

void setup() {
    Serial.begin(115200);
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_wifi_connects_successfully);
    RUN_TEST(test_wifi_has_valid_ip);
    UNITY_END();
}

void loop() {}
