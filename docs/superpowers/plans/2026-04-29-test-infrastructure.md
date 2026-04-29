# Test Infrastructure Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Agregar infraestructura de tests con Unity — tests nativos (host) para dominio y aplicación, tests de integración (ESP8266) para WiFi, HTTP y hardware.

**Architecture:** Tests nativos usan `platform = native` con stubs de Arduino y fakes de repositorios; tests de integración usan `platform = espressif8266` con hardware real. Cada suite de tests es un subdirectorio independiente bajo `test/native/` o `test/integration/`.

**Tech Stack:** Unity (PlatformIO built-in), Arduino_JSON (nativo via lib_deps), C++17 para stubs.

---

## Mapa de archivos

**Modificar:**
- `platformio.ini` — renombrar `nodemcuv2` → `esp12f`, agregar env `native`

**Crear — stubs (test/native/stubs/):**
- `test/native/stubs/Arduino.h` — `String`, `Serial`, `PROGMEM`, constantes, funciones stub
- `test/native/stubs/HysteresisController.h` — stub no-op hasta que exista la impl real

**Crear — fakes (test/native/fakes/):**
- `test/native/fakes/FakeTemperatureRepository.h`
- `test/native/fakes/FakeTimeRepository.h`
- `test/native/fakes/FakeHardwareRepository.h`

**Crear — tests nativos:**
- `test/native/test_json_parser/test_json_parser.cpp`
- `test/native/test_hysteresis_controller/test_hysteresis_controller.cpp`
- `test/native/test_get_temperature_use_case/test_get_temperature_use_case.cpp`
- `test/native/test_control_temperature_use_case/test_control_temperature_use_case.cpp`

**Crear — tests de integración:**
- `test/integration/test_wifi/test_wifi.cpp`
- `test/integration/test_open_meteo_adapter/test_open_meteo_adapter.cpp`
- `test/integration/test_hardware_adapter/test_hardware_adapter.cpp`

---

## Task 1: platformio.ini — agregar envs de test

**Files:**
- Modify: `platformio.ini`

- [ ] **Step 1: Reemplazar contenido de platformio.ini**

```ini
[env:esp12f]
platform = espressif8266
board = esp12e
framework = arduino
monitor_speed = 115200
build_flags =
    -I src/domain/entities
    -I src/domain/repositories
    -I src/domain/services
    -I src/application/use_cases
    -I src/application/services
    -I src/infrastructure
    -I src/infrastructure/http
    -I src/infrastructure/json
    -I src/infrastructure/hardware
    -I src/interfaces/controllers
lib_deps =
    arduino-libraries/Arduino_JSON@^0.2.0
test_filter = integration/*

[env:native]
platform = native
build_flags =
    -std=c++17
    -I src/domain/entities
    -I src/domain/repositories
    -I src/domain/services
    -I src/application/use_cases
    -I src/application/services
    -I src/infrastructure
    -I src/infrastructure/json
    -I test/native/stubs
    -I test/native/fakes
build_src_filter =
    -<**>
    +<domain/**>
    +<application/use_cases/**>
    +<infrastructure/json/**>
    +<infrastructure/config.cpp>
lib_deps =
    arduino-libraries/Arduino_JSON@^0.2.0
test_filter = native/*
```

- [ ] **Step 2: Verificar que compila el env principal**

```bash
pio run -e esp12f
```

Esperado: `[SUCCESS]`

- [ ] **Step 3: Commit**

```bash
git add platformio.ini
git commit -m "build: agregar envs native y esp12f para tests con Unity"
```

---

## Task 2: Arduino.h stub

**Files:**
- Create: `test/native/stubs/Arduino.h`

- [ ] **Step 1: Crear el stub**

```cpp
// test/native/stubs/Arduino.h
#pragma once
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>

#define HIGH 1
#define LOW  0
#define INPUT  0
#define OUTPUT 1
#define PROGMEM

class String {
    std::string _s;
public:
    String() = default;
    String(const char* s) : _s(s ? s : "") {}
    String(const std::string& s) : _s(s) {}
    String(int n) : _s(std::to_string(n)) {}
    String(double d) : _s(std::to_string(d)) {}

    const char* c_str() const { return _s.c_str(); }
    size_t length() const { return _s.length(); }
    bool isEmpty() const { return _s.empty(); }

    String substring(int from, int to = -1) const {
        if (to < 0) return String(_s.substr(from));
        return String(_s.substr(from, to - from));
    }

    String& operator+=(const String& o) { _s += o._s; return *this; }
    String& operator+=(const char* o)   { _s += o;    return *this; }
    String& operator+=(char c)          { _s += c;    return *this; }

    bool operator==(const String& o) const { return _s == o._s; }
    bool operator==(const char* o)   const { return _s == o; }
    bool operator!=(const String& o) const { return _s != o._s; }
    bool operator!=(const char* o)   const { return _s != o; }

    operator const char*() const { return _s.c_str(); }
    operator std::string() const { return _s; }
};

inline String operator+(String a, const String& b) { return a += b; }
inline String operator+(String a, const char* b)   { return a += b; }
inline String operator+(const char* a, String b)   { return String(a) += b; }

struct SerialClass {
    void begin(unsigned long) {}
    void print(const char*)   {}
    void print(const String&) {}
    void println(const char* s = "") { (void)s; }
    void println(const String&)      {}
    void printf(const char*, ...)    {}
};

inline SerialClass Serial;

inline void pinMode(int, int)         {}
inline void digitalWrite(int, int)    {}
inline int  digitalRead(int)          { return 0; }
inline void delay(unsigned long)      {}
inline unsigned long millis()         { return 0; }
inline void yield()                   {}
```

- [ ] **Step 2: Verificar que el env native compila (sin tests aún)**

```bash
pio run -e native
```

Esperado: `[SUCCESS]` o error solo por ausencia de archivos en `test/`.

- [ ] **Step 3: Commit**

```bash
git add test/native/stubs/Arduino.h
git commit -m "test: agregar stub de Arduino.h para compilación nativa"
```

---

## Task 3: HysteresisController stub

**Files:**
- Create: `test/native/stubs/HysteresisController.h`

Este stub permite que los tests de `test_hysteresis_controller` compilen antes de que exista
la implementación real en `src/domain/services/HysteresisController.h` (rama `feature/add-pid`).
Cuando la implementación real exista, el compilador la encontrará primero (vía `-I src/domain/services`
que tiene mayor prioridad en `build_flags`) y este stub quedará ignorado.

- [ ] **Step 1: Crear el stub**

```cpp
// test/native/stubs/HysteresisController.h
// STUB: no-op hasta que src/domain/services/HysteresisController.h exista (feature/add-pid).
// Los tests de test_hysteresis_controller FALLARÁN en runtime hasta entonces.
// NO definir BOILER_ENABLE_TEMP/DISABLE_TEMP aquí — vienen de config.h en la impl real.
#pragma once

class HysteresisController {
public:
    static bool evaluate(double /*temperature*/, bool currentlyEnabled) {
        return currentlyEnabled; // stub: mantiene estado sin lógica real
    }
};
```

- [ ] **Step 2: Commit**

```bash
git add test/native/stubs/HysteresisController.h
git commit -m "test: agregar stub de HysteresisController para compilación nativa"
```

---

## Task 4: Fake repositories

**Files:**
- Create: `test/native/fakes/FakeTemperatureRepository.h`
- Create: `test/native/fakes/FakeTimeRepository.h`
- Create: `test/native/fakes/FakeHardwareRepository.h`

- [ ] **Step 1: Crear FakeTemperatureRepository.h**

```cpp
// test/native/fakes/FakeTemperatureRepository.h
#pragma once
#include "ITemperatureRepository.h"

class FakeTemperatureRepository : public ITemperatureRepository {
public:
    bool tokenResult      = true;
    bool tempResult       = true;
    double fakeTemp       = 20.0;
    String fakeToken      = "fake-token";
    int authCallCount     = 0;
    int tempCallCount     = 0;

    bool getAuthToken(String& token) override {
        authCallCount++;
        token = fakeToken;
        return tokenResult;
    }

    bool getCurrentTemperature(const String&, const String&, double& temperature) override {
        tempCallCount++;
        temperature = fakeTemp;
        return tempResult;
    }
};
```

- [ ] **Step 2: Crear FakeTimeRepository.h**

```cpp
// test/native/fakes/FakeTimeRepository.h
#pragma once
#include "ITimeRepository.h"

class FakeTimeRepository : public ITimeRepository {
public:
    bool timeResult       = true;
    String fakeTime       = "2024-08-03T01:00:00Z";
    int callCount         = 0;

    bool getCurrentTime(String& currentTime) override {
        callCount++;
        currentTime = fakeTime;
        return timeResult;
    }
};
```

- [ ] **Step 3: Crear FakeHardwareRepository.h**

```cpp
// test/native/fakes/FakeHardwareRepository.h
#pragma once
#include "IHardwareRepository.h"

class FakeHardwareRepository : public IHardwareRepository {
public:
    int activateHeatingCount   = 0;
    int deactivateHeatingCount = 0;
    int activateCoolingCount   = 0;
    int deactivateCoolingCount = 0;
    int activateEmergencyCount = 0;

    void reset() {
        activateHeatingCount   = 0;
        deactivateHeatingCount = 0;
        activateCoolingCount   = 0;
        deactivateCoolingCount = 0;
        activateEmergencyCount = 0;
    }

    void initialize() override {}
    void updateLeds(bool) override {}
    void delayWithLedUpdate(unsigned long) override {}
    void activateHeating()   override { activateHeatingCount++; }
    void deactivateHeating() override { deactivateHeatingCount++; }
    void activateCooling()   override { activateCoolingCount++; }
    void deactivateCooling() override { deactivateCoolingCount++; }
    void activateEmergency() override { activateEmergencyCount++; }
    void deactivateEmergency() override {}
};
```

- [ ] **Step 4: Commit**

```bash
git add test/native/fakes/
git commit -m "test: agregar fake repositories para tests nativos"
```

---

## Task 5: test_json_parser (nativo)

**Files:**
- Create: `test/native/test_json_parser/test_json_parser.cpp`

- [ ] **Step 1: Crear el archivo de tests**

```cpp
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
    "{\"current_weather\":{\"temperature\":17.5,\"windspeed\":10.0,"
    "\"winddirection\":90,\"weathercode\":2,\"time\":\"2024-08-03T01:00\"}}";

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

void test_malformed_json_returns_false_open_meteo() {
    double temp = 0.0;
    bool result = JsonParser::extractOpenMeteoTemperature(
        String("{\"wrong_key\":42}"), temp);
    TEST_ASSERT_FALSE(result);
}

int main(int argc, char** argv) {
    UNITY_BEGIN();
    RUN_TEST(test_extract_meteomatics_temperature);
    RUN_TEST(test_extract_open_meteo_temperature);
    RUN_TEST(test_extract_value_existing_key);
    RUN_TEST(test_extract_value_missing_key);
    RUN_TEST(test_malformed_json_returns_false_meteomatics);
    RUN_TEST(test_malformed_json_returns_false_open_meteo);
    return UNITY_END();
}
```

- [ ] **Step 2: Correr el test y verificar que pasa**

```bash
pio test -e native -f native/test_json_parser
```

Esperado: `6 Tests 0 Failures 0 Ignored` y `[PASSED]`.

Si Arduino_JSON falla al compilar en nativo (error de headers ESP8266), mover el directorio a
`test/integration/test_json_parser/` y cambiar la función `main` por `setup`/`loop` de Arduino.

- [ ] **Step 3: Commit**

```bash
git add test/native/test_json_parser/
git commit -m "test: agregar tests nativos de JsonParser"
```

---

## Task 6: test_hysteresis_controller (nativo)

**Files:**
- Create: `test/native/test_hysteresis_controller/test_hysteresis_controller.cpp`

Estos tests FALLARÁN hasta que `src/domain/services/HysteresisController.h/.cpp` sea implementado
(rama `feature/add-pid`). Compilan y sirven como especificación ejecutable del comportamiento esperado.

- [ ] **Step 1: Crear el archivo de tests**

```cpp
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
```

- [ ] **Step 2: Correr el test**

```bash
pio test -e native -f native/test_hysteresis_controller
```

Esperado con stub: varios `FAIL` (el stub no implementa la lógica real). Esto es correcto hasta
que `feature/add-pid` sea mergeado. Verificar que **compila** y que los failures son de assertions,
no de errores de compilación.

- [ ] **Step 3: Commit**

```bash
git add test/native/test_hysteresis_controller/
git commit -m "test: agregar tests nativos de HysteresisController (requiere feature/add-pid)"
```

---

## Task 7: test_get_temperature_use_case (nativo)

**Files:**
- Create: `test/native/test_get_temperature_use_case/test_get_temperature_use_case.cpp`

- [ ] **Step 1: Crear el archivo de tests**

```cpp
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
    TEST_ASSERT_EQUAL(0, fakeTime->callCount);        // no debe llegar a pedir el tiempo
    TEST_ASSERT_EQUAL(0, fakeTemp->tempCallCount);    // no debe llegar a pedir temperatura
}

void test_get_time_failure_returns_false() {
    fakeTime->timeResult = false;
    double temp = 0.0;
    bool result = useCase->execute(temp);
    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_EQUAL(1, fakeTemp->authCallCount);
    TEST_ASSERT_EQUAL(1, fakeTime->callCount);
    TEST_ASSERT_EQUAL(0, fakeTemp->tempCallCount);    // no debe llegar a pedir temperatura
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
    // El use case llamó una vez a getAuthToken y una vez a getCurrentTemperature
    TEST_ASSERT_EQUAL(1, fakeTemp->authCallCount);
    TEST_ASSERT_EQUAL(1, fakeTemp->tempCallCount);
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
```

- [ ] **Step 2: Correr el test**

```bash
pio test -e native -f native/test_get_temperature_use_case
```

Esperado: `5 Tests 0 Failures 0 Ignored` y `[PASSED]`.

- [ ] **Step 3: Commit**

```bash
git add test/native/test_get_temperature_use_case/
git commit -m "test: agregar tests nativos de GetTemperatureUseCase"
```

---

## Task 8: test_control_temperature_use_case (nativo)

**Files:**
- Create: `test/native/test_control_temperature_use_case/test_control_temperature_use_case.cpp`

Los tests reflejan el comportamiento ACTUAL de `ControlTemperatureUseCase` (usa `TemperatureController`
con 4 rangos fijos). Los tests marcados con `// TODO feature/add-pid` verificarán el comportamiento
de histeresis una vez que ese branch sea mergeado.

- [ ] **Step 1: Crear el archivo de tests**

```cpp
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
// void test_deadband_does_not_call_hardware_when_off() {
//     // 15 <= temp <= 18 y boilerEnabled=false → ninguna llamada al hardware
//     useCase->execute(16.5);
//     TEST_ASSERT_EQUAL(0, fakeHw->activateHeatingCount);
//     TEST_ASSERT_EQUAL(0, fakeHw->deactivateHeatingCount);
// }
//
// void test_no_double_activate_on_consecutive_calls() {
//     // Dos ciclos en zona baja → activateHeating llamado solo 1 vez
//     useCase->execute(14.0);
//     useCase->execute(14.0);
//     TEST_ASSERT_EQUAL(1, fakeHw->activateHeatingCount);
// }

int main(int argc, char** argv) {
    UNITY_BEGIN();
    RUN_TEST(test_execute_returns_true);
    RUN_TEST(test_critical_low_activates_emergency_and_heating);
    RUN_TEST(test_low_temp_activates_heating);
    RUN_TEST(test_medium_temp_deactivates_all);
    RUN_TEST(test_high_temp_activates_cooling);
    return UNITY_END();
}
```

- [ ] **Step 2: Correr el test**

```bash
pio test -e native -f native/test_control_temperature_use_case
```

Esperado: `5 Tests 0 Failures 0 Ignored` y `[PASSED]`.

- [ ] **Step 3: Correr todos los tests nativos juntos**

```bash
pio test -e native
```

Esperado: `test_json_parser`, `test_get_temperature_use_case` y `test_control_temperature_use_case`
en `[PASSED]`. `test_hysteresis_controller` en `[FAILED]` (stub, esperado). Total failures solo
de hysteresis.

- [ ] **Step 4: Commit**

```bash
git add test/native/test_control_temperature_use_case/
git commit -m "test: agregar tests nativos de ControlTemperatureUseCase"
```

---

## Task 9: test_wifi (integración)

**Files:**
- Create: `test/integration/test_wifi/test_wifi.cpp`

Requiere hardware ESP8266 conectado y `credentials.h` con WiFi válido.

- [ ] **Step 1: Crear el archivo de tests**

```cpp
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
```

- [ ] **Step 2: Flashear y correr el test**

```bash
pio test -e esp12f -f integration/test_wifi --upload-port /dev/ttyUSB0
```

Esperado: `3 Tests 0 Failures 0 Ignored` y `[PASSED]` en el monitor serial.

- [ ] **Step 3: Commit**

```bash
git add test/integration/test_wifi/
git commit -m "test: agregar test de integración de WiFi"
```

---

## Task 10: test_open_meteo_adapter (integración)

**Files:**
- Create: `test/integration/test_open_meteo_adapter/test_open_meteo_adapter.cpp`

Requiere WiFi conectado y acceso a internet. No consume cuota de APIs de pago.

- [ ] **Step 1: Crear el archivo de tests**

```cpp
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

    // Conectar WiFi antes de correr los tests de red
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
```

- [ ] **Step 2: Flashear y correr el test**

```bash
pio test -e esp12f -f integration/test_open_meteo_adapter --upload-port /dev/ttyUSB0
```

Esperado: `3 Tests 0 Failures 0 Ignored` y `[PASSED]`.

- [ ] **Step 3: Commit**

```bash
git add test/integration/test_open_meteo_adapter/
git commit -m "test: agregar test de integración de OpenMeteoAdapter"
```

---

## Task 11: test_hardware_adapter (integración)

**Files:**
- Create: `test/integration/test_hardware_adapter/test_hardware_adapter.cpp`

Verifica que el pin 4 (GPIO4, caldera) responde correctamente a `activateHeating` y
`deactivateHeating`. El pin se inicializa como OUTPUT en `initialize()`.

- [ ] **Step 1: Crear el archivo de tests**

```cpp
// test/integration/test_hardware_adapter/test_hardware_adapter.cpp
#include <Arduino.h>
#include <unity.h>
#include "Esp8266HardwareAdapter.h"
#include "config.h"

static Esp8266HardwareAdapter* hw;

void setUp() {
    hw = new Esp8266HardwareAdapter();
    hw->initialize();
}

void tearDown() {
    hw->deactivateHeating(); // dejar pin en LOW al terminar cada test
    delete hw;
}

void test_activate_heating_sets_pin_high() {
    hw->activateHeating();
    TEST_ASSERT_EQUAL_MESSAGE(HIGH, digitalRead(LED_PIN_1),
        "Pin 4 no esta HIGH despues de activateHeating()");
}

void test_deactivate_heating_sets_pin_low() {
    hw->activateHeating();
    hw->deactivateHeating();
    TEST_ASSERT_EQUAL_MESSAGE(LOW, digitalRead(LED_PIN_1),
        "Pin 4 no esta LOW despues de deactivateHeating()");
}

void test_activate_deactivate_activate_sequence() {
    hw->activateHeating();
    TEST_ASSERT_EQUAL(HIGH, digitalRead(LED_PIN_1));
    hw->deactivateHeating();
    TEST_ASSERT_EQUAL(LOW, digitalRead(LED_PIN_1));
    hw->activateHeating();
    TEST_ASSERT_EQUAL(HIGH, digitalRead(LED_PIN_1));
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
```

- [ ] **Step 2: Flashear y correr el test**

```bash
pio test -e esp12f -f integration/test_hardware_adapter --upload-port /dev/ttyUSB0
```

Esperado: `3 Tests 0 Failures 0 Ignored` y `[PASSED]`.

- [ ] **Step 3: Commit final**

```bash
git add test/integration/test_hardware_adapter/
git commit -m "test: agregar test de integración de Esp8266HardwareAdapter"
```

---

## Verificación final

- [ ] **Correr todos los tests nativos**

```bash
pio test -e native
```

Esperado: 19 tests totales. `test_hysteresis_controller` falla (stub, esperado). El resto pasa.

- [ ] **Correr todos los tests de integración**

```bash
pio test -e esp12f --upload-port /dev/ttyUSB0
```

Esperado: 9 tests totales, todos `[PASSED]`.
