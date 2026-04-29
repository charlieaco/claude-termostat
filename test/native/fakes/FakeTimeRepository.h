// test/native/fakes/FakeTimeRepository.h
#pragma once
#include "ITimeRepository.h"

class FakeTimeRepository : public ITimeRepository {
public:
    bool timeResult       = true;
    String fakeTime       = "2024-08-03T01:00:00Z";
    int callCount         = 0;

    bool getCurrentTime(String& currentTime) override {
        callCount++;
        currentTime = fakeTime;
        return timeResult;
    }
};
