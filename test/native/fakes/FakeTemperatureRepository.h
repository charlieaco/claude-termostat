// test/native/fakes/FakeTemperatureRepository.h
#pragma once
#include "ITemperatureRepository.h"

class FakeTemperatureRepository : public ITemperatureRepository {
public:
    bool tokenResult      = true;
    bool tempResult       = true;
    double fakeTemp       = 20.0;
    String fakeToken      = "fake-token";
    int authCallCount     = 0;
    int tempCallCount     = 0;

    bool getAuthToken(String& token) override {
        authCallCount++;
        token = fakeToken;
        return tokenResult;
    }

    bool getCurrentTemperature(const String&, const String&, double& temperature) override {
        tempCallCount++;
        temperature = fakeTemp;
        return tempResult;
    }
};
