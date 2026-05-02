# Apparent Temperature Design Spec

**Date:** 2026-05-01
**Status:** Approved

---

## Context

El adaptador Open-Meteo obtiene `temperature_2m` (temperatura del aire) para alimentar el algoritmo de control de caldera. La sensación térmica (`apparent_temperature`) es más relevante para el confort humano ya que incorpora viento, humedad y radiación. La API de Open-Meteo retorna ambas variables en el mismo request sin costo adicional.

---

## Goal

Reemplazar `temperature_2m` por `apparent_temperature` como dato que alimenta el algoritmo de control, manteniendo ambos valores visibles en el log.

---

## Approach: Cambio transparente en la infraestructura

La interfaz `ITemperatureRepository` no cambia — `getCurrentTemperature()` sigue retornando un `double`. El cambio vive enteramente en `OpenMeteoAdapter` y `JsonParser`. El dominio, los use cases, los fakes y los tests de lógica no se tocan.

---

## Files Changed

### `src/infrastructure/http/OpenMeteoAdapter.cpp`

**URL** — agregar `apparent_temperature` al parámetro `current`:

```cpp
"&current=temperature_2m,apparent_temperature"
"&timezone=America%2FArgentina%2FBuenos_Aires"
```

**`getCurrentTemperature()`** — parsear ambas variables, loguear las dos, retornar `apparent_temperature`:

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

### `src/infrastructure/json/JsonParser.h`

Agregar declaracion del nuevo metodo:

```cpp
static bool extractOpenMeteoApparentTemperature(const String& jsonData, double& apparentTemp);
```

### `src/infrastructure/json/JsonParser.cpp`

Implementar el nuevo metodo, simetrico a `extractOpenMeteoTemperature`:

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

---

## Testing

### Nativo

Agregar un test en `test/native/test_json_parser/test_json_parser.cpp`:

```cpp
static const char* OPEN_METEO_RESPONSE_WITH_APPARENT =
    "{\"current\":{\"time\":\"2026-05-01T10:00\","
    "\"temperature_2m\":14.2,\"apparent_temperature\":11.8}}";

void test_extract_open_meteo_apparent_temperature() {
    double temp = 0.0;
    bool result = JsonParser::extractOpenMeteoApparentTemperature(
        String(OPEN_METEO_RESPONSE_WITH_APPARENT), temp);
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 11.8, temp);
}
```

### Integracion

`test_open_meteo_adapter` ya verifica que `getCurrentTemperature` retorna true y un valor en rango plausible — sigue siendo valido para `apparent_temperature`.

---

## Out of Scope

- Cambios en `ITemperatureRepository`, use cases, fakes, `ThermostatController`.
- Logica de control basada en viento por separado.
- Historial de temperatura del aire.
