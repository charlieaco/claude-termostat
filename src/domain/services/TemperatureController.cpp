#include "TemperatureController.h"
#include "config.h"

TemperatureController::TemperatureRange TemperatureController::evaluateTemperature(double temperature) {
    if (temperature < TEMPERATURE_LOW_THRESHOLD) {
        return TemperatureRange::CRITICAL_LOW;
    } else if (temperature < TEMPERATURE_MEDIUM_THRESHOLD) {
        return TemperatureRange::TEMP_LOW;
    } else if (temperature < TEMPERATURE_HIGH_THRESHOLD) {
        return TemperatureRange::MEDIUM;
    } else {
        return TemperatureRange::TEMP_HIGH;
    }
}

void TemperatureController::executeTemperatureAction(TemperatureRange range, double temperature) {
    Serial.printf("Executing action for temperature %.2f°C (Range: %s)\n", 
                  temperature, getTemperatureRangeDescription(range).c_str());
    
    switch (range) {
        case TemperatureRange::CRITICAL_LOW:
            handleCriticalLowTemperature(temperature);
            break;
        case TemperatureRange::TEMP_LOW:
            handleLowTemperature(temperature);
            break;
        case TemperatureRange::MEDIUM:
            handleMediumTemperature(temperature);
            break;
        case TemperatureRange::TEMP_HIGH:
            handleHighTemperature(temperature);
            break;
    }
}

String TemperatureController::getTemperatureRangeDescription(TemperatureRange range) {
    switch (range) {
        case TemperatureRange::CRITICAL_LOW:
            return "Critical Low (< 5°C)";
        case TemperatureRange::TEMP_LOW:
            return "Low (5-10°C)";
        case TemperatureRange::MEDIUM:
            return "Medium (10-16°C)";
        case TemperatureRange::TEMP_HIGH:
            return "High (> 16°C)";
        default:
            return "Unknown";
    }
}

void TemperatureController::handleCriticalLowTemperature(double temperature) {
    Serial.println("CRITICAL: Temperature is very low - Activating emergency heating");
    // TODO: Implement emergency heating logic
    // digitalWrite(HEATING_PIN, HIGH);
    // digitalWrite(EMERGENCY_LED_PIN, HIGH);
}

void TemperatureController::handleLowTemperature(double temperature) {
    Serial.println("LOW: Temperature is low - Activating heating system");
    // TODO: Implement heating logic
    // digitalWrite(HEATING_PIN, HIGH);
    // digitalWrite(HEATING_LED_PIN, HIGH);
}

void TemperatureController::handleMediumTemperature(double temperature) {
    Serial.println("MEDIUM: Temperature is comfortable - Maintaining current settings");
    // TODO: Implement maintenance logic
    // digitalWrite(HEATING_PIN, LOW);
    // digitalWrite(COOLING_PIN, LOW);
}

void TemperatureController::handleHighTemperature(double temperature) {
    Serial.println("HIGH: Temperature is high - Activating cooling system");
    // TODO: Implement cooling logic
    // digitalWrite(COOLING_PIN, HIGH);
    // digitalWrite(COOLING_LED_PIN, HIGH);
} 