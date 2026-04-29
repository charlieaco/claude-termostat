# Hysteresis Boiler Control — Design Spec

**Date:** 2026-04-22  
**Branch:** feature/add-pid  
**Status:** Approved

---

## Context

The thermostat reads exterior temperature from the Meteomatics API every ~2 minutes. The ESP8266 is installed next to the boiler; local temperature is irrelevant. The goal is to authorize or block the boiler from firing based on outdoor conditions.

The boiler has its own internal hydraulic thermostat that fires every ~2 minutes when enabled. Our device controls a single digital pin (pin 4) that acts as an authorization gate: HIGH = boiler allowed to fire, LOW = boiler blocked.

---

## Decision

Replace the current 4-range fixed-threshold logic with a **bang-bang controller with hysteresis** (Option B from brainstorming):

- Enable boiler when exterior temperature drops **below 15°C**
- Disable boiler when exterior temperature rises **above 18°C**
- Between 15°C and 18°C: **maintain current state** (no pin change)

This deadband of 3°C prevents rapid toggling when temperature oscillates around a single threshold.

---

## Architecture

### New domain service: `HysteresisController`

Pure logic, no side effects, no hardware dependencies.

```cpp
// src/domain/services/HysteresisController.h
class HysteresisController {
public:
    static bool evaluate(double temperature, bool currentlyEnabled);
};
```

Rules:
- `temperature < BOILER_ENABLE_TEMP (15.0)` → return `true`
- `temperature > BOILER_DISABLE_TEMP (18.0)` → return `false`
- otherwise → return `currentlyEnabled`

### Modified use case: `ControlTemperatureUseCase`

Gains a persistent `boilerEnabled` bool member (initialized to `false`). On each `execute()` call:

1. Call `HysteresisController::evaluate(temperature, boilerEnabled)`
2. If result differs from `boilerEnabled` → call `activateHeating()` or `deactivateHeating()`
3. Update `boilerEnabled`
4. Log current state via Serial

Hardware is only touched when state changes — no spurious pin toggling every cycle.

### Hardware adapter: `Esp8266HardwareAdapter`

Implement the pending TODOs:
- `activateHeating()` → `digitalWrite(LED_PIN_1, HIGH)` (pin 4)
- `deactivateHeating()` → `digitalWrite(LED_PIN_1, LOW)` (pin 4)

Cooling and emergency methods remain as stubs (no hardware connected).

### Config changes

Replace:
```cpp
#define TEMPERATURE_HIGH_THRESHOLD 16.0
#define TEMPERATURE_MEDIUM_THRESHOLD 10.0
#define TEMPERATURE_LOW_THRESHOLD 5.0
```

With:
```cpp
#define BOILER_ENABLE_TEMP  15.0
#define BOILER_DISABLE_TEMP 18.0
```

---

## Data Flow

```
Meteomatics API → exterior temperature
        ↓
HysteresisController::evaluate(temp, boilerEnabled)
        ↓
  state changed?
    YES → activateHeating() or deactivateHeating()
          update boilerEnabled
    NO  → no hardware call
        ↓
Serial log of current state and temperature
```

---

## Error Handling

If the API fails, `getTemperatureUseCase.execute()` returns false and `handleEvaluatingDecisionState()` is never reached — the boiler pin is not touched. The boiler maintains its last authorized state. This behavior is already correct in the existing state machine and requires no changes.

**Initial state:** `boilerEnabled = false` — boiler starts blocked until the first successful temperature read. This is safe.

---

## Files Changed

| Action | File |
|--------|------|
| Create | `src/domain/services/HysteresisController.h` |
| Create | `src/domain/services/HysteresisController.cpp` |
| Modify | `src/infrastructure/config.h` |
| Modify | `src/application/use_cases/ControlTemperatureUseCase.h` |
| Modify | `src/application/use_cases/ControlTemperatureUseCase.cpp` |
| Modify | `src/infrastructure/hardware/Esp8266HardwareAdapter.cpp` |
| Delete | `src/domain/services/TemperatureController.h` |
| Delete | `src/domain/services/TemperatureController.cpp` |

Files not touched: state machine, WiFi adapter, HTTP adapters, `ThermostatController`, `main.cpp`.
