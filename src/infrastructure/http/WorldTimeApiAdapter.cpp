#include "WorldTimeApiAdapter.h"

bool WorldTimeApiAdapter::getCurrentTime(String& currentTime) {
    if (!HttpClient::getCurrentTime(currentTime)) {
        Serial.println("Failed to get current time from WorldTime API");
        return false;
    }

    Serial.printf("Current time obtained from WorldTime API: %s\n", currentTime.c_str());
    return true;
}
 