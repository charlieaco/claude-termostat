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
#include "infrastructure/http/MeteoApiAdapter.h"
#include "infrastructure/http/WorldTimeApiAdapter.h"
#include "infrastructure/hardware/Esp8266HardwareAdapter.h"
#include "infrastructure/hardware/Esp8266WifiAdapter.h"

// Interfaces Layer
#include "interfaces/controllers/ThermostatController.h"

// Infrastructure Layer instances
MeteoApiAdapter meteoApiAdapter;
WorldTimeApiAdapter worldTimeApiAdapter;
Esp8266HardwareAdapter hardwareAdapter;
Esp8266WifiAdapter wifiAdapter;

// Application Layer instances
GetTemperatureUseCase getTemperatureUseCase(meteoApiAdapter, worldTimeApiAdapter);
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
