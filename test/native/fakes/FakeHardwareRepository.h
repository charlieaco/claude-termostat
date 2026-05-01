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
    int deactivateEmergencyCount = 0;

    void reset() {
        activateHeatingCount   = 0;
        deactivateHeatingCount = 0;
        activateCoolingCount   = 0;
        deactivateCoolingCount = 0;
        activateEmergencyCount = 0;
        deactivateEmergencyCount = 0;
    }

    void initialize() override {}
    void updateLeds(bool) override {}
    void sleep(unsigned long) override {}
    void activateHeating()   override { activateHeatingCount++; }
    void deactivateHeating() override { deactivateHeatingCount++; }
    void activateCooling()   override { activateCoolingCount++; }
    void deactivateCooling() override { deactivateCoolingCount++; }
    void activateEmergency() override { activateEmergencyCount++; }
    void deactivateEmergency() override { deactivateEmergencyCount++; }
};
