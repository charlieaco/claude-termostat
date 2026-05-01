// test/native/test_control_temperature_use_case/test_control_temperature_use_case.cpp
#include <unity.h>
#include "ControlTemperatureUseCase.h"
#include "FakeHardwareRepository.h"

static FakeHardwareRepository* fakeHw;
static ControlTemperatureUseCase* useCase;

void setUp() {
    fakeHw  = new FakeHardwareRepository();
    useCase = new ControlTemperatureUseCase(*fakeHw);
}

void tearDown() {
    delete useCase;
    delete fakeHw;
}

void test_execute_returns_true() {
    bool result = useCase->execute(13.0);
    TEST_ASSERT_TRUE(result);
}

void test_critical_low_activates_emergency_and_heating() {
    // temp < 5°C → CRITICAL_LOW → activateEmergency + activateHeating
    useCase->execute(2.0);
    TEST_ASSERT_EQUAL(1, fakeHw->activateEmergencyCount);
    TEST_ASSERT_EQUAL(1, fakeHw->activateHeatingCount);
}

void test_low_temp_activates_heating() {
    // 5 <= temp < 10 → TEMP_LOW → activateHeating + deactivateCooling
    useCase->execute(7.0);
    TEST_ASSERT_EQUAL(1, fakeHw->activateHeatingCount);
    TEST_ASSERT_EQUAL(0, fakeHw->activateCoolingCount);
    TEST_ASSERT_EQUAL(1, fakeHw->deactivateCoolingCount);
}

void test_medium_temp_deactivates_all() {
    // 10 <= temp < 16 → MEDIUM → deactivateHeating + deactivateCooling
    useCase->execute(13.0);
    TEST_ASSERT_EQUAL(0, fakeHw->activateHeatingCount);
    TEST_ASSERT_EQUAL(0, fakeHw->activateCoolingCount);
    TEST_ASSERT_EQUAL(1, fakeHw->deactivateHeatingCount);
    TEST_ASSERT_EQUAL(1, fakeHw->deactivateCoolingCount);
}

void test_high_temp_activates_cooling() {
    // temp >= 16 → TEMP_HIGH → activateCooling + deactivateHeating
    useCase->execute(20.0);
    TEST_ASSERT_EQUAL(1, fakeHw->activateCoolingCount);
    TEST_ASSERT_EQUAL(1, fakeHw->deactivateHeatingCount);
    TEST_ASSERT_EQUAL(0, fakeHw->activateHeatingCount);
}

// TODO feature/add-pid: descomentar cuando ControlTemperatureUseCase use HysteresisController
// void test_deadband_does_not_call_hardware_when_off() { ... }
// void test_no_double_activate_on_consecutive_calls() { ... }

int main(int argc, char** argv) {
    UNITY_BEGIN();
    RUN_TEST(test_execute_returns_true);
    RUN_TEST(test_critical_low_activates_emergency_and_heating);
    RUN_TEST(test_low_temp_activates_heating);
    RUN_TEST(test_medium_temp_deactivates_all);
    RUN_TEST(test_high_temp_activates_cooling);
    return UNITY_END();
}
