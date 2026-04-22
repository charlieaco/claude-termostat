#ifndef TEMPERATURE_CONTROLLER_H
#define TEMPERATURE_CONTROLLER_H

#include <Arduino.h>

class TemperatureController {
public:
    enum class TemperatureRange {
        CRITICAL_LOW,    // < 5°C
        TEMP_LOW,        // 5-10°C
        MEDIUM,          // 10-16°C
        TEMP_HIGH        // > 16°C
    };
    
    static TemperatureRange evaluateTemperature(double temperature);
    static void executeTemperatureAction(TemperatureRange range, double temperature);
    static String getTemperatureRangeDescription(TemperatureRange range);
    
private:
    static void handleCriticalLowTemperature(double temperature);
    static void handleLowTemperature(double temperature);
    static void handleMediumTemperature(double temperature);
    static void handleHighTemperature(double temperature);
};

#endif // TEMPERATURE_CONTROLLER_H 