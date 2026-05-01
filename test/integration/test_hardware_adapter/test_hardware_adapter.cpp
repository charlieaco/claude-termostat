// test/integration/test_hardware_adapter/test_hardware_adapter.cpp
#include <Arduino.h>
#include <unity.h>
#include "Esp8266HardwareAdapter.h"
#include "config.h"

// NOTA: En esta branch, activateHeating/deactivateHeating no escriben a ningun GPIO
// (Esp8266HardwareAdapter.cpp tiene la llamada comentada). test_activate_heating_sets_pin_high
// FALLARA en hardware hasta que feature/add-sleep-mode sea mergeado, donde activateHeating
// escribe HIGH a LED_PIN_2 y deactivateHeating escribe LOW.

static Esp8266HardwareAdapter* hw;

void setUp() {
    hw = new Esp8266HardwareAdapter();
    hw->initialize();
}

void tearDown() {
    hw->deactivateHeating();
    delete hw;
}

void test_activate_heating_sets_pin_high() {
    hw->activateHeating();
    TEST_ASSERT_EQUAL_MESSAGE(HIGH, digitalRead(LED_PIN_2),
        "LED_PIN_2 no esta HIGH despues de activateHeating()");
}

void test_deactivate_heating_sets_pin_low() {
    hw->activateHeating();
    hw->deactivateHeating();
    TEST_ASSERT_EQUAL_MESSAGE(LOW, digitalRead(LED_PIN_2),
        "LED_PIN_2 no esta LOW despues de deactivateHeating()");
}

void test_activate_deactivate_activate_sequence() {
    hw->activateHeating();
    TEST_ASSERT_EQUAL(HIGH, digitalRead(LED_PIN_2));
    hw->deactivateHeating();
    TEST_ASSERT_EQUAL(LOW, digitalRead(LED_PIN_2));
    hw->activateHeating();
    TEST_ASSERT_EQUAL(HIGH, digitalRead(LED_PIN_2));
}

void setup() {
    Serial.begin(115200);
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_activate_heating_sets_pin_high);
    RUN_TEST(test_deactivate_heating_sets_pin_low);
    RUN_TEST(test_activate_deactivate_activate_sequence);
    UNITY_END();
}

void loop() {}
