#ifndef OPEN_METEO_ADAPTER_H
#define OPEN_METEO_ADAPTER_H

#include "../../domain/repositories/ITemperatureRepository.h"
#include "../config.h"
#include <Arduino.h>

class OpenMeteoAdapter : public ITemperatureRepository {
public:
    OpenMeteoAdapter() = default;
    ~OpenMeteoAdapter() override = default;

    bool getCurrentTemperature(const String& timestamp, const String& authToken, double& temperature) override;
    bool getAuthToken(String& token) override;
};

#endif // OPEN_METEO_ADAPTER_H
