#include "GetTemperatureUseCase.h"

bool GetTemperatureUseCase::execute(double& temperature) {
    String authToken;
    String currentTime;
    
    // Obtener token de autenticación
    if (!getAuthToken(authToken)) {
        Serial.println("Failed to get authentication token");
        return false;
    }
    
    // Obtener hora actual (no fatal: providers como Open-Meteo no la necesitan)
    if (!getCurrentTime(currentTime)) {
        Serial.println("Warning: failed to get current time, proceeding with empty timestamp");
    }
    
    // Obtener temperatura
    if (!temperatureRepository.getCurrentTemperature(currentTime, authToken, temperature)) {
        Serial.println("Failed to get temperature data");
        return false;
    }
    
    Serial.printf("Temperature obtained successfully: %.2f°C\n", temperature);
    return true;
}

bool GetTemperatureUseCase::getAuthToken(String& token) {
    return temperatureRepository.getAuthToken(token);
}

bool GetTemperatureUseCase::getCurrentTime(String& currentTime) {
    return timeRepository.getCurrentTime(currentTime);
} 