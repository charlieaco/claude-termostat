// test/native/test_get_temperature_use_case/test_get_temperature_use_case.cpp
#include <unity.h>
#include "GetTemperatureUseCase.h"
#include "FakeTemperatureRepository.h"
#include "FakeTimeRepository.h"

static FakeTemperatureRepository* fakeTemp;
static FakeTimeRepository* fakeTime;
static GetTemperatureUseCase* useCase;

void setUp() {
    fakeTemp = new FakeTemperatureRepository();
    fakeTime = new FakeTimeRepository();
    useCase  = new GetTemperatureUseCase(*fakeTemp, *fakeTime);
}

void tearDown() {
    delete useCase;
    delete fakeTime;
    delete fakeTemp;
}

void test_happy_path_returns_true_and_temperature() {
    fakeTemp->fakeTemp = 13.5;
    double temp = 0.0;
    bool result = useCase->execute(temp);
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 13.5, temp);
}

void test_auth_token_failure_returns_false() {
    fakeTemp->tokenResult = false;
    double temp = 0.0;
    bool result = useCase->execute(temp);
    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_EQUAL(1, fakeTemp->authCallCount);
    TEST_ASSERT_EQUAL(0, fakeTime->callCount);
    TEST_ASSERT_EQUAL(0, fakeTemp->tempCallCount);
}

void test_get_time_failure_returns_false() {
    fakeTime->timeResult = false;
    double temp = 0.0;
    bool result = useCase->execute(temp);
    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_EQUAL(1, fakeTemp->authCallCount);
    TEST_ASSERT_EQUAL(1, fakeTime->callCount);
    TEST_ASSERT_EQUAL(0, fakeTemp->tempCallCount);
}

void test_get_temperature_failure_returns_false() {
    fakeTemp->tempResult = false;
    double temp = 0.0;
    bool result = useCase->execute(temp);
    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_EQUAL(1, fakeTemp->authCallCount);
    TEST_ASSERT_EQUAL(1, fakeTime->callCount);
    TEST_ASSERT_EQUAL(1, fakeTemp->tempCallCount);
}

void test_auth_token_passed_to_temperature_call() {
    fakeTemp->fakeToken = "my-token";
    double temp = 0.0;
    useCase->execute(temp);
    TEST_ASSERT_EQUAL_STRING("my-token", fakeTemp->lastReceivedToken.c_str());
}

int main(int argc, char** argv) {
    UNITY_BEGIN();
    RUN_TEST(test_happy_path_returns_true_and_temperature);
    RUN_TEST(test_auth_token_failure_returns_false);
    RUN_TEST(test_get_time_failure_returns_false);
    RUN_TEST(test_get_temperature_failure_returns_false);
    RUN_TEST(test_auth_token_passed_to_temperature_call);
    return UNITY_END();
}
