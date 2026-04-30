#include "OpenMeteoAdapter.h"
#include "../json/JsonParser.h"
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

bool OpenMeteoAdapter::getAuthToken(String& token) {
    token = "";
    return true;
}

bool OpenMeteoAdapter::getCurrentTemperature(const String&, const String&, double& temperature) {
    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
    client->setInsecure();
    Serial.println("OpenMeteo: getting temperature");

    HTTPClient https;
    String url = String("https://") + OPEN_METEO_HOST +
                 "/v1/forecast?latitude=" + LOCATION_LATITUDE +
                 "&longitude=" + LOCATION_LONGITUDE +
                 "&current=temperature_2m";

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

    String response = https.getString();
    https.end();

    if (!JsonParser::extractOpenMeteoTemperature(response, temperature)) {
        Serial.println("OpenMeteo: failed to parse temperature");
        return false;
    }

    Serial.printf("Temperature obtained from Open-Meteo: %.2f°C\n", temperature);
    return true;
}
