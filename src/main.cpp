#include <Arduino.h>

#include "infrastructure/config.h"

// Domain Layer
#include "domain/entities/ThermostatState.h"
#include "domain/repositories/ITemperatureRepository.h"
#include "domain/repositories/ITimeRepository.h"
#include "domain/repositories/IWifiRepository.h"
#include "domain/repositories/IHardwareRepository.h"
#include "domain/services/TemperatureController.h"

// Application Layer
#include "application/use_cases/GetTemperatureUseCase.h"
#include "application/use_cases/ControlTemperatureUseCase.h"

// Infrastructure Layer
#include "infrastructure/http/HttpClient.h"
#include "infrastructure/json/JsonParser.h"
#include "infrastructure/hardware/Esp8266HardwareAdapter.h"
#include "infrastructure/hardware/Esp8266WifiAdapter.h"

#if TEMPERATURE_PROVIDER == TEMPERATURE_PROVIDER_OPEN_METEO
#include "infrastructure/http/OpenMeteoAdapter.h"
#else
#include "infrastructure/http/MeteoApiAdapter.h"
#include "infrastructure/http/WorldTimeApiAdapter.h"
#endif

// Interfaces Layer
#include "interfaces/controllers/ThermostatController.h"

// Infrastructure Layer instances
#if TEMPERATURE_PROVIDER == TEMPERATURE_PROVIDER_OPEN_METEO
OpenMeteoAdapter temperatureAdapter;
#else
MeteoApiAdapter temperatureAdapter;
WorldTimeApiAdapter worldTimeApiAdapter;
#endif
Esp8266HardwareAdapter hardwareAdapter;
Esp8266WifiAdapter wifiAdapter;

// Application Layer instances
#if TEMPERATURE_PROVIDER == TEMPERATURE_PROVIDER_OPEN_METEO
GetTemperatureUseCase getTemperatureUseCase(temperatureAdapter, temperatureAdapter);
#else
GetTemperatureUseCase getTemperatureUseCase(temperatureAdapter, worldTimeApiAdapter);
#endif
ControlTemperatureUseCase controlTemperatureUseCase(hardwareAdapter);

// Interfaces Layer instance
ThermostatController thermostatController(
    wifiAdapter,
    getTemperatureUseCase,
    controlTemperatureUseCase,
    hardwareAdapter
);

void setup() {
    thermostatController.initialize();
}

void loop() {
    thermostatController.run();
}
