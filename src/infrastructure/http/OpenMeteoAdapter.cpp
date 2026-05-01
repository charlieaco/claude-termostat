#include "OpenMeteoAdapter.h"
#include "../json/JsonParser.h"
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

bool OpenMeteoAdapter::getAuthToken(String& token) {
    token = "";
    return true;
}

bool OpenMeteoAdapter::fetchResponse(String& response) {
    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
    client->setInsecure();

    HTTPClient https;
    String url = String("https://") + OPEN_METEO_HOST +
                 "/v1/forecast?latitude=" + LOCATION_LATITUDE +
                 "&longitude=" + LOCATION_LONGITUDE +
                 "&current=temperature_2m"
                 "&timezone=America%2FArgentina%2FBuenos_Aires";

    if (!https.begin(*client, url)) {
        Serial.println("OpenMeteo: connection failed");
        return false;
    }

    int httpCode = https.GET();
    if (httpCode != HTTP_CODE_OK) {
        Serial.printf("OpenMeteo: HTTP error %d\n", httpCode);
        https.end();
        return false;
    }

    response = https.getString();
    https.end();
    return true;
}

bool OpenMeteoAdapter::getCurrentTime(String& currentTime) {
    if (!fetchResponse(_cachedResponse)) {
        _cachedResponse = "";
        return false;
    }

    if (!JsonParser::extractOpenMeteoTime(_cachedResponse, currentTime)) {
        Serial.println("OpenMeteo: failed to parse time");
        _cachedResponse = "";
        return false;
    }

    Serial.printf("Time obtained from Open-Meteo: %s\n", currentTime.c_str());
    return true;
}

bool OpenMeteoAdapter::getCurrentTemperature(const String&, const String&, double& temperature) {
    Serial.println("OpenMeteo: getting temperature");

    String response;
    if (_cachedResponse.length() > 0) {
        response = _cachedResponse;
        _cachedResponse = "";
    } else {
        if (!fetchResponse(response)) return false;
    }

    if (!JsonParser::extractOpenMeteoTemperature(response, temperature)) {
        Serial.println("OpenMeteo: failed to parse temperature");
        return false;
    }

    Serial.printf("Temperature obtained from Open-Meteo: %.2f°C\n", temperature);
    return true;
}
