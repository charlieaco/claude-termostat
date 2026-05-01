// test/native/fakes/FakeTemperatureRepository.h
#pragma once
#include "ITemperatureRepository.h"

class FakeTemperatureRepository : public ITemperatureRepository {
public:
    bool tokenResult      = true;
    bool tempResult       = true;
    double fakeTemp       = 20.0;
    String fakeToken      = "fake-token";
    String lastReceivedToken;
    int authCallCount     = 0;
    int tempCallCount     = 0;

    bool getAuthToken(String& token) override {
        authCallCount++;
        token = fakeToken;
        return tokenResult;
    }

    bool getCurrentTemperature(const String&, const String& token, double& temperature) override {
        tempCallCount++;
        lastReceivedToken = token;
        temperature = fakeTemp;
        return tempResult;
    }
};
