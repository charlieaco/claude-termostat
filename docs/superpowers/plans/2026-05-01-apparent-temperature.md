# Apparent Temperature Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Replace `temperature_2m` with `apparent_temperature` as the value returned by `OpenMeteoAdapter::getCurrentTemperature()`, while logging both values.

**Architecture:** The change lives entirely in the infrastructure layer — `JsonParser` and `OpenMeteoAdapter`. `ITemperatureRepository` and all domain/application code are untouched. A new `JsonParser::extractOpenMeteoApparentTemperature()` method is added, symmetric to the existing `extractOpenMeteoTemperature()`. The URL is extended to request both fields in the same HTTP call.

**Tech Stack:** C++17 (native tests), Arduino/ESP8266 (device), PlatformIO, Unity (test framework), Arduino_JSON library.

---

## File Map

- Modify: `src/infrastructure/json/JsonParser.h` — add declaration for `extractOpenMeteoApparentTemperature`
- Modify: `src/infrastructure/json/JsonParser.cpp` — implement `extractOpenMeteoApparentTemperature`
- Modify: `src/infrastructure/http/OpenMeteoAdapter.cpp` — extend URL, parse both temps, log both, return apparent
- Modify: `test/native/test_json_parser/test_json_parser.cpp` — add test for new parser method

---

### Task 1: JsonParser — extractOpenMeteoApparentTemperature

**Files:**
- Modify: `src/infrastructure/json/JsonParser.h`
- Modify: `src/infrastructure/json/JsonParser.cpp`
- Modify: `test/native/test_json_parser/test_json_parser.cpp`

- [ ] **Step 1: Add the failing test**

Add the constant and test function to `test/native/test_json_parser/test_json_parser.cpp`, and register it in `main()`:

```cpp
// After the OPEN_METEO_RESPONSE constant (line 16), add:
static const char* OPEN_METEO_RESPONSE_WITH_APPARENT =
    "{\"current\":{\"time\":\"2026-05-01T10:00\","
    "\"temperature_2m\":14.2,\"apparent_temperature\":11.8}}";

// After test_open_meteo_malformed_json_returns_false (before main), add:
void test_extract_open_meteo_apparent_temperature() {
    double temp = 0.0;
    bool result = JsonParser::extractOpenMeteoApparentTemperature(
        String(OPEN_METEO_RESPONSE_WITH_APPARENT), temp);
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 11.8, temp);
}
```

In `main()`, add before `return UNITY_END();`:
```cpp
    RUN_TEST(test_extract_open_meteo_apparent_temperature);
```

- [ ] **Step 2: Run test to verify it fails**

```bash
pio test -e native --filter native/test_json_parser -v 2>&1 | tail -20
```

Expected: compile error — `extractOpenMeteoApparentTemperature` not declared.

- [ ] **Step 3: Declare the method in the header**

In `src/infrastructure/json/JsonParser.h`, add after line 12 (`extractOpenMeteoTime`):

```cpp
    static bool extractOpenMeteoApparentTemperature(const String& jsonData, double& apparentTemp);
```

Full `public` section after the change:
```cpp
public:
    static bool extractValue(const String& jsonString, const String& key, String& result);
    static bool extractTemperatureValue(const String& jsonData, double& temperature);
    static bool extractOpenMeteoTemperature(const String& jsonData, double& temperature);
    static bool extractOpenMeteoApparentTemperature(const String& jsonData, double& apparentTemp);
    static bool extractOpenMeteoTime(const String& jsonData, String& time);
    static void printJsonStructure(const String& jsonString);
```

- [ ] **Step 4: Implement the method in JsonParser.cpp**

Add after `extractOpenMeteoTemperature` (after line 91, before `extractOpenMeteoTime`):

```cpp
bool JsonParser::extractOpenMeteoApparentTemperature(const String& jsonData, double& apparentTemp) {
    JSONVar jsonObject = JSON.parse(jsonData);

    if (!isValidJson(jsonObject)) {
        Serial.println("OpenMeteo JSON parsing failed (apparent_temperature)!");
        return false;
    }

    JSONVar current = jsonObject["current"];
    if (JSON.typeof(current) == "undefined") {
        Serial.println("OpenMeteo: current not found (apparent_temperature)");
        return false;
    }

    apparentTemp = (double)current["apparent_temperature"];
    return true;
}
```

- [ ] **Step 5: Run test to verify it passes**

```bash
pio test -e native --filter native/test_json_parser -v 2>&1 | tail -20
```

Expected: all 8 tests pass, including `test_extract_open_meteo_apparent_temperature`.

- [ ] **Step 6: Commit**

```bash
git add src/infrastructure/json/JsonParser.h \
        src/infrastructure/json/JsonParser.cpp \
        test/native/test_json_parser/test_json_parser.cpp
git commit -m "feat: agregar extractOpenMeteoApparentTemperature a JsonParser"
```

---

### Task 2: OpenMeteoAdapter — URL y lógica de getCurrentTemperature

**Files:**
- Modify: `src/infrastructure/http/OpenMeteoAdapter.cpp`

- [ ] **Step 1: Update the URL in `fetchResponse()`**

In `src/infrastructure/http/OpenMeteoAdapter.cpp`, line 19, change:

```cpp
                 "&current=temperature_2m"
```

to:

```cpp
                 "&current=temperature_2m,apparent_temperature"
```

- [ ] **Step 2: Update `getCurrentTemperature()` to parse both and return apparent**

Replace the body of `getCurrentTemperature()` (lines 55–73) with:

```cpp
bool OpenMeteoAdapter::getCurrentTemperature(const String&, const String&, double& temperature) {
    Serial.println("OpenMeteo: getting temperature");

    String response;
    if (_cachedResponse.length() > 0) {
        response = _cachedResponse;
        _cachedResponse = "";
    } else {
        if (!fetchResponse(response)) return false;
    }

    double airTemp = 0.0;
    double apparentTemp = 0.0;

    if (!JsonParser::extractOpenMeteoTemperature(response, airTemp)) {
        Serial.println("OpenMeteo: failed to parse temperature_2m");
        return false;
    }

    if (!JsonParser::extractOpenMeteoApparentTemperature(response, apparentTemp)) {
        Serial.println("OpenMeteo: failed to parse apparent_temperature");
        return false;
    }

    Serial.printf("OpenMeteo: temperatura_2m=%.2f°C  apparent=%.2f°C\n", airTemp, apparentTemp);
    temperature = apparentTemp;
    return true;
}
```

- [ ] **Step 3: Run the full native test suite to confirm no regressions**

```bash
pio test -e native -v 2>&1 | tail -30
```

Expected: all native tests pass.

- [ ] **Step 4: Commit**

```bash
git add src/infrastructure/http/OpenMeteoAdapter.cpp
git commit -m "feat: usar apparent_temperature como dato de control, loguear ambas temperaturas"
```
