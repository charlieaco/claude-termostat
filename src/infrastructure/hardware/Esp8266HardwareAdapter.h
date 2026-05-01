#ifndef ESP8266_HARDWARE_ADAPTER_H
#define ESP8266_HARDWARE_ADAPTER_H

#include "../../domain/repositories/IHardwareRepository.h"
#include "../config.h"
#include <Arduino.h>

class Esp8266HardwareAdapter : public IHardwareRepository {
public:
    Esp8266HardwareAdapter() = default;
    ~Esp8266HardwareAdapter() override = default;

    void initialize() override;
    void updateLeds(bool status) override;
    void sleep(unsigned long milliseconds) override;
    void activateHeating() override;
    void deactivateHeating() override;
    void activateCooling() override;
    void deactivateCooling() override;
    void activateEmergency() override;
    void deactivateEmergency() override;
};

#endif // ESP8266_HARDWARE_ADAPTER_H
