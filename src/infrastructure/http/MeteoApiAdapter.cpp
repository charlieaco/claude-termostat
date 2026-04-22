#include "MeteoApiAdapter.h"

bool MeteoApiAdapter::getCurrentTemperature(const String& timestamp, const String& authToken, double& temperature) {
    String temperatureResponse;
    
    // Obtener datos de temperatura desde la API
    if (!HttpClient::getTemperatureData(timestamp, authToken, temperatureResponse)) {
        Serial.println("Failed to get temperature data from Meteomatics API");
        return false;
    }
    
    // Extraer el valor de temperatura del JSON
    if (!JsonParser::extractTemperatureValue(temperatureResponse, temperature)) {
        Serial.println("Failed to parse temperature data from API response");
        return false;
    }
    
    Serial.printf("Temperature obtained from Meteomatics API: %.2f°C\n", temperature);
    return true;
}

bool MeteoApiAdapter::getAuthToken(String& token) {
    return HttpClient::getAuthToken(token);
}

String MeteoApiAdapter::buildTemperatureUrl(const String& timestamp, const String& token) {
    String url = "/";
    url += timestamp;
    url += "/t_2m:C/";
    url += LOCATION_LATITUDE;
    url += ",";
    url += LOCATION_LONGITUDE;
    url += "/json?access_token=";
    url += token;
    return url;
} 