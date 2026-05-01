#ifndef OPEN_METEO_ADAPTER_H
#define OPEN_METEO_ADAPTER_H

#include "../../domain/repositories/ITemperatureRepository.h"
#include "../../domain/repositories/ITimeRepository.h"
#include "../config.h"
#include <Arduino.h>

class OpenMeteoAdapter : public ITemperatureRepository, public ITimeRepository {
public:
    OpenMeteoAdapter() = default;
    ~OpenMeteoAdapter() override = default;

    bool getAuthToken(String& token) override;
    bool getCurrentTemperature(const String& timestamp, const String& authToken, double& temperature) override;
    bool getCurrentTime(String& currentTime) override;

private:
    String _cachedResponse;
    bool fetchResponse(String& response);
};

#endif // OPEN_METEO_ADAPTER_H
