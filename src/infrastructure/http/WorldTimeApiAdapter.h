#ifndef WORLD_TIME_API_ADAPTER_H
#define WORLD_TIME_API_ADAPTER_H

#include "../../domain/repositories/ITimeRepository.h"
#include "HttpClient.h"
#include "../config.h"
#include <Arduino.h>

class WorldTimeApiAdapter : public ITimeRepository {
public:
    WorldTimeApiAdapter() = default;
    ~WorldTimeApiAdapter() override = default;

    bool getCurrentTime(String& currentTime) override;
};

#endif // WORLD_TIME_API_ADAPTER_H 