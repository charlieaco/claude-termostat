// test/native/test_hysteresis_controller/test_hysteresis_controller.cpp
// NOTA: requiere feature/add-pid mergeado para que los tests pasen.
// Actualmente usa el stub en test/native/stubs/HysteresisController.h.
#include <unity.h>
#include "HysteresisController.h"

void setUp() {}
void tearDown() {}

void test_enable_when_below_threshold_starting_disabled() {
    // temp < 15 y estado previo = false → debe habilitar
    bool result = HysteresisController::evaluate(14.0, false);
    TEST_ASSERT_TRUE(result);
}

void test_enable_when_below_threshold_starting_enabled() {
    // temp < 15 y estado previo = true → mantiene habilitado
    bool result = HysteresisController::evaluate(14.0, true);
    TEST_ASSERT_TRUE(result);
}

void test_disable_when_above_threshold_starting_enabled() {
    // temp > 18 y estado previo = true → debe deshabilitar
    bool result = HysteresisController::evaluate(19.0, true);
    TEST_ASSERT_FALSE(result);
}

void test_disable_when_above_threshold_starting_disabled() {
    // temp > 18 y estado previo = false → mantiene deshabilitado
    bool result = HysteresisController::evaluate(19.0, false);
    TEST_ASSERT_FALSE(result);
}

void test_deadband_maintains_enabled_state() {
    // 15 <= temp <= 18 y estado = true → mantiene true
    bool result = HysteresisController::evaluate(16.5, true);
    TEST_ASSERT_TRUE(result);
}

void test_deadband_maintains_disabled_state() {
    // 15 <= temp <= 18 y estado = false → mantiene false
    bool result = HysteresisController::evaluate(16.5, false);
    TEST_ASSERT_FALSE(result);
}

void test_exact_lower_boundary() {
    // temp == 15.0 exacto → zona muerta, mantiene estado
    TEST_ASSERT_FALSE(HysteresisController::evaluate(15.0, false));
    TEST_ASSERT_TRUE(HysteresisController::evaluate(15.0, true));
}

void test_exact_upper_boundary() {
    // temp == 18.0 exacto → zona muerta, mantiene estado
    TEST_ASSERT_FALSE(HysteresisController::evaluate(18.0, false));
    TEST_ASSERT_TRUE(HysteresisController::evaluate(18.0, true));
}

int main(int argc, char** argv) {
    UNITY_BEGIN();
    RUN_TEST(test_enable_when_below_threshold_starting_disabled);
    RUN_TEST(test_enable_when_below_threshold_starting_enabled);
    RUN_TEST(test_disable_when_above_threshold_starting_enabled);
    RUN_TEST(test_disable_when_above_threshold_starting_disabled);
    RUN_TEST(test_deadband_maintains_enabled_state);
    RUN_TEST(test_deadband_maintains_disabled_state);
    RUN_TEST(test_exact_lower_boundary);
    RUN_TEST(test_exact_upper_boundary);
    return UNITY_END();
}
