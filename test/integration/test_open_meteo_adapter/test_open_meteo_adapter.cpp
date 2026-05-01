// test/integration/test_open_meteo_adapter/test_open_meteo_adapter.cpp
#include <Arduino.h>
#include <unity.h>
#include "Esp8266WifiAdapter.h"
#include "OpenMeteoAdapter.h"

void setUp() {}
void tearDown() {}

void test_get_auth_token_is_noop_and_returns_true() {
    OpenMeteoAdapter adapter;
    String token = "initial";
    bool result = adapter.getAuthToken(token);
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_STRING("", token.c_str());
}

void test_get_temperature_returns_true() {
    OpenMeteoAdapter adapter;
    double temp = -999.0;
    bool result = adapter.getCurrentTemperature("", "", temp);
    TEST_ASSERT_TRUE_MESSAGE(result, "Open-Meteo no respondio — verificar conexion WiFi");
}

void test_temperature_is_in_plausible_range() {
    OpenMeteoAdapter adapter;
    double temp = -999.0;
    adapter.getCurrentTemperature("", "", temp);
    TEST_ASSERT_MESSAGE(temp > -50.0 && temp < 60.0,
        "Temperatura fuera de rango plausible (-50 a 60°C)");
}

void setup() {
    Serial.begin(115200);
    delay(2000);

    Esp8266WifiAdapter wifi;
    if (!wifi.initialize()) {
        Serial.println("SKIP: WiFi no disponible");
        UNITY_BEGIN();
        UNITY_END();
        return;
    }

    UNITY_BEGIN();
    RUN_TEST(test_get_auth_token_is_noop_and_returns_true);
    RUN_TEST(test_get_temperature_returns_true);
    RUN_TEST(test_temperature_is_in_plausible_range);
    UNITY_END();
}

void loop() {}
