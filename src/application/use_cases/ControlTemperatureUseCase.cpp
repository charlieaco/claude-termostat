#include "ControlTemperatureUseCase.h"

bool ControlTemperatureUseCase::execute(double temperature) {
    Serial.printf("Evaluating temperature: %.2f°C\n", temperature);
    
    // Evaluar el rango de temperatura
    auto temperatureRange = TemperatureController::evaluateTemperature(temperature);
    
    // Ejecutar la acción correspondiente
    executeTemperatureAction(temperatureRange, temperature);
    
    Serial.println("Temperature control executed successfully");
    return true;
}

void ControlTemperatureUseCase::executeTemperatureAction(TemperatureController::TemperatureRange range, double temperature) {
    Serial.printf("Executing action for temperature %.2f°C (Range: %s)\n", 
                  temperature, TemperatureController::getTemperatureRangeDescription(range).c_str());
    
    switch (range) {
        case TemperatureController::TemperatureRange::CRITICAL_LOW:
            Serial.println("CRITICAL: Temperature is very low - Activating emergency heating");
            hardwareRepository.activateEmergency();
            hardwareRepository.activateHeating();
            break;
            
        case TemperatureController::TemperatureRange::TEMP_LOW:
            Serial.println("LOW: Temperature is low - Activating heating system");
            hardwareRepository.activateHeating();
            hardwareRepository.deactivateCooling();
            break;
            
        case TemperatureController::TemperatureRange::MEDIUM:
            Serial.println("MEDIUM: Temperature is comfortable - Maintaining current settings");
            hardwareRepository.deactivateHeating();
            hardwareRepository.deactivateCooling();
            break;
            
        case TemperatureController::TemperatureRange::TEMP_HIGH:
            Serial.println("HIGH: Temperature is high - Activating cooling system");
            hardwareRepository.activateCooling();
            hardwareRepository.deactivateHeating();
            break;
    }
} 