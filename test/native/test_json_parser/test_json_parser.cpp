// test/native/test_json_parser/test_json_parser.cpp
#include <unity.h>
#include "JsonParser.h"

void setUp() {}
void tearDown() {}

// Respuesta Meteomatics con temperatura 14.2
static const char* METEOMATICS_RESPONSE =
    "{\"data\":[{\"coordinates\":[{\"dates\":[{\"date\":\"2024-08-03T00:00:00Z\","
    "\"value\":14.2}],\"lat\":-34.4,\"lon\":-58.7}],\"parameter\":\"t_2m:C\"}]}";

// Respuesta Open-Meteo con temperatura 17.5
static const char* OPEN_METEO_RESPONSE =
    "{\"current\":{\"temperature_2m\":17.5,\"wind_speed_10m\":10.0,"
    "\"time\":\"2024-08-03T01:00\"}}";

// Respuesta Open-Meteo con temperatura aparente 11.8
static const char* OPEN_METEO_RESPONSE_WITH_APPARENT =
    "{\"current\":{\"time\":\"2026-05-01T10:00\","
    "\"temperature_2m\":14.2,\"apparent_temperature\":11.8}}";

void test_extract_meteomatics_temperature() {
    double temp = 0.0;
    bool result = JsonParser::extractTemperatureValue(
        String(METEOMATICS_RESPONSE), temp);
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 14.2, temp);
}

void test_extract_open_meteo_temperature() {
    double temp = 0.0;
    bool result = JsonParser::extractOpenMeteoTemperature(
        String(OPEN_METEO_RESPONSE), temp);
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 17.5, temp);
}

void test_extract_value_existing_key() {
    String response = "{\"access_token\":\"abc123\"}";
    String value;
    bool result = JsonParser::extractValue(response, "access_token", value);
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_STRING("abc123", value.c_str());
}

void test_extract_value_missing_key() {
    String response = "{\"other_key\":\"value\"}";
    String value;
    bool result = JsonParser::extractValue(response, "access_token", value);
    TEST_ASSERT_FALSE(result);
}

void test_malformed_json_returns_false_meteomatics() {
    double temp = 0.0;
    bool result = JsonParser::extractTemperatureValue(
        String("not-json-at-all{{{"), temp);
    TEST_ASSERT_FALSE(result);
}

void test_open_meteo_missing_key_returns_false() {
    double temp = 0.0;
    bool result = JsonParser::extractOpenMeteoTemperature(
        String("{\"wrong_key\":42}"), temp);
    TEST_ASSERT_FALSE(result);
}

void test_open_meteo_malformed_json_returns_false() {
    double temp = 0.0;
    bool result = JsonParser::extractOpenMeteoTemperature(
        String("{{{not-valid"), temp);
    TEST_ASSERT_FALSE(result);
}

void test_extract_open_meteo_apparent_temperature() {
    double temp = 0.0;
    bool result = JsonParser::extractOpenMeteoApparentTemperature(
        String(OPEN_METEO_RESPONSE_WITH_APPARENT), temp);
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 11.8, temp);
}

void test_open_meteo_apparent_temperature_missing_key_returns_false() {
    double temp = 0.0;
    bool result = JsonParser::extractOpenMeteoApparentTemperature(
        String("{\"current\":{\"temperature_2m\":14.2}}"), temp);
    TEST_ASSERT_FALSE(result);
}

int main(int argc, char** argv) {
    UNITY_BEGIN();
    RUN_TEST(test_extract_meteomatics_temperature);
    RUN_TEST(test_extract_open_meteo_temperature);
    RUN_TEST(test_extract_value_existing_key);
    RUN_TEST(test_extract_value_missing_key);
    RUN_TEST(test_malformed_json_returns_false_meteomatics);
    RUN_TEST(test_open_meteo_missing_key_returns_false);
    RUN_TEST(test_open_meteo_malformed_json_returns_false);
    RUN_TEST(test_extract_open_meteo_apparent_temperature);
    RUN_TEST(test_open_meteo_apparent_temperature_missing_key_returns_false);
    return UNITY_END();
}
