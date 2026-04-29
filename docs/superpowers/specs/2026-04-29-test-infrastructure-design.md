# Test Infrastructure — Design Spec

**Date:** 2026-04-29  
**Status:** Approved

---

## Context

The project implements a hexagonal architecture ESP8266 thermostat. The domain layer is pure C++; the infrastructure layer depends on Arduino, WiFi, and HTTP. Tests are split into two environments: native (host machine, fast iteration) and integration (on-device, real hardware).

---

## Framework

**Unity** for both environments. PlatformIO supports it out-of-the-box via `pio test`. One framework, consistent API (`TEST_ASSERT_EQUAL`, `TEST_ASSERT_TRUE`, etc.), no extra dependencies.

---

## Directory Structure

```
test/
├── native/
│   ├── stubs/
│   │   └── Arduino.h                         # String → std::string, Serial → no-op
│   ├── fakes/
│   │   ├── FakeTemperatureRepository.h
│   │   ├── FakeTimeRepository.h
│   │   └── FakeHardwareRepository.h
│   ├── test_json_parser/
│   │   └── test_json_parser.cpp
│   ├── test_hysteresis_controller/
│   │   └── test_hysteresis_controller.cpp
│   ├── test_get_temperature_use_case/
│   │   └── test_get_temperature_use_case.cpp
│   └── test_control_temperature_use_case/
│       └── test_control_temperature_use_case.cpp
└── integration/
    ├── test_wifi/
    │   └── test_wifi.cpp
    ├── test_open_meteo_adapter/
    │   └── test_open_meteo_adapter.cpp
    └── test_hardware_adapter/
        └── test_hardware_adapter.cpp
```

---

## PlatformIO Configuration

Add to `platformio.ini`:

```ini
[env:native]
platform = native
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
    -I test/native/stubs
    -I test/native/fakes
test_filter = native/*

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
```

### Running tests

```bash
pio test -e native                  # host, sin hardware
pio test -e esp12f                  # dispositivo, requiere USB + credentials.h
```

---

## Arduino Stub (`test/native/stubs/Arduino.h`)

Permite compilar código que incluye `<Arduino.h>` en host sin modificar archivos de producción:

- `String` → `std::string` con los métodos usados en el proyecto (`c_str()`, `length()`, `+=`, `printf`-compatible)
- `Serial.print` / `Serial.println` / `Serial.printf` → no-op macros
- Constantes Arduino necesarias (`HIGH`, `LOW`, `INPUT`, `OUTPUT`)

---

## Native Tests

### Fake repositories

Clases C++ que implementan las interfaces del dominio. Exponen campos públicos para configurar el comportamiento y contadores para verificar llamadas:

```cpp
class FakeTemperatureRepository : public ITemperatureRepository {
public:
    bool tokenResult = true;
    bool tempResult = true;
    double fakeTemperature = 20.0;
    int getAuthTokenCallCount = 0;
    int getCurrentTemperatureCallCount = 0;

    bool getAuthToken(String& token) override { ... }
    bool getCurrentTemperature(const String&, const String&, double& t) override { ... }
};
```

Mismo patrón para `FakeTimeRepository` y `FakeHardwareRepository`.

### `test_json_parser`

| Test | Descripción |
|------|-------------|
| `test_extract_meteomatics_temperature` | JSON válido de Meteomatics → temperatura correcta |
| `test_extract_open_meteo_temperature` | JSON válido de Open-Meteo → temperatura correcta |
| `test_extract_value_existing_key` | `extractValue()` con clave presente → retorna true |
| `test_extract_value_missing_key` | `extractValue()` con clave ausente → retorna false |
| `test_malformed_json_returns_false` | JSON malformado → retorna false sin crash |

### `test_hysteresis_controller`

Depende de que `HysteresisController` esté implementado (rama `feature/add-pid`).

| Test | Condición | Esperado |
|------|-----------|----------|
| `test_enable_below_threshold` | temp = 14.0, enabled = false | true |
| `test_enable_below_threshold_already_on` | temp = 14.0, enabled = true | true |
| `test_disable_above_threshold` | temp = 19.0, enabled = true | false |
| `test_disable_above_threshold_already_off` | temp = 19.0, enabled = false | false |
| `test_deadband_maintains_enabled` | temp = 16.5, enabled = true | true |
| `test_deadband_maintains_disabled` | temp = 16.5, enabled = false | false |

### `test_get_temperature_use_case`

| Test | Condición | Esperado |
|------|-----------|----------|
| `test_happy_path` | todos los fakes retornan true | retorna true, temperatura correcta |
| `test_auth_token_fails` | `tokenResult = false` | retorna false |
| `test_get_time_fails` | `timeResult = false` | retorna false |
| `test_get_temperature_fails` | `tempResult = false` | retorna false |

### `test_control_temperature_use_case`

Depende de que `ControlTemperatureUseCase` use `HysteresisController` (rama `feature/add-pid`).

| Test | Condición | Esperado |
|------|-----------|----------|
| `test_activates_below_threshold` | temp = 14.0 | `activateHeating` llamado 1 vez |
| `test_no_call_in_deadband_when_off` | temp = 16.5, estado inicial off | ningún método llamado |
| `test_deactivates_above_threshold` | temp = 19.0, estado previo on | `deactivateHeating` llamado 1 vez |
| `test_no_double_activate` | dos ciclos con temp = 14.0 | `activateHeating` llamado 1 vez (no 2) |

---

## Integration Tests

Requieren hardware ESP8266 conectado por USB y `credentials.h` con WiFi válido.

### `test_wifi`

| Test | Esperado |
|------|----------|
| `test_wifi_connects` | conexión exitosa en menos de 10 segundos |
| `test_wifi_gets_ip` | IP distinta de `0.0.0.0` |

### `test_open_meteo_adapter`

Prerrequisito: WiFi conectado.

| Test | Esperado |
|------|----------|
| `test_get_auth_token_is_noop` | retorna true, token vacío |
| `test_get_temperature_returns_true` | retorna true |
| `test_temperature_in_plausible_range` | temperatura entre -50°C y 60°C |

### `test_hardware_adapter`

| Test | Esperado |
|------|----------|
| `test_activate_sets_pin_high` | `digitalRead(4)` == HIGH después de `activateHeating()` |
| `test_deactivate_sets_pin_low` | `digitalRead(4)` == LOW después de `deactivateHeating()` |
| `test_activate_deactivate_sequence` | secuencia activate → deactivate → activate funciona |

---

## Error Handling

- Tests de integración que fallan por falta de WiFi o credenciales dan mensaje claro via `Serial` antes de fallar el assert.
- `HysteresisController` no existe aún en `main` — los tests que dependen de él se marcan con comentario `// requiere feature/add-pid mergeado`.

---

## Files Changed

| Acción | Archivo |
|--------|---------|
| Modify | `platformio.ini` |
| Create | `test/native/stubs/Arduino.h` |
| Create | `test/native/fakes/FakeTemperatureRepository.h` |
| Create | `test/native/fakes/FakeTimeRepository.h` |
| Create | `test/native/fakes/FakeHardwareRepository.h` |
| Create | `test/native/test_json_parser/test_json_parser.cpp` |
| Create | `test/native/test_hysteresis_controller/test_hysteresis_controller.cpp` |
| Create | `test/native/test_get_temperature_use_case/test_get_temperature_use_case.cpp` |
| Create | `test/native/test_control_temperature_use_case/test_control_temperature_use_case.cpp` |
| Create | `test/integration/test_wifi/test_wifi.cpp` |
| Create | `test/integration/test_open_meteo_adapter/test_open_meteo_adapter.cpp` |
| Create | `test/integration/test_hardware_adapter/test_hardware_adapter.cpp` |
