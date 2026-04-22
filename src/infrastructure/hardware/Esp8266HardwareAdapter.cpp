#include "Esp8266HardwareAdapter.h"

void Esp8266HardwareAdapter::initialize() {
    // Configurar pines de salida
    pinMode(LED_PIN_1, OUTPUT);
    pinMode(LED_PIN_2, OUTPUT);
    
    // Inicializar LEDs apagados
    digitalWrite(LED_PIN_1, LOW);
    digitalWrite(LED_PIN_2, LOW);
    
    Serial.println("ESP8266 hardware initialized successfully");
}

void Esp8266HardwareAdapter::updateLeds(bool status) {
    digitalWrite(LED_PIN_1, status);
    digitalWrite(LED_PIN_2, status);
}

void Esp8266HardwareAdapter::delayWithLedUpdate(unsigned long milliseconds) {
    unsigned long startTime = millis();
    while (millis() - startTime < milliseconds) {
        toggleLeds();
        delay(500); // Update LEDs every 500ms
    }
}

void Esp8266HardwareAdapter::activateHeating() {
    Serial.println("Hardware: Activating heating system");
    // TODO: Implementar control de calefacción
    // digitalWrite(HEATING_PIN, HIGH);
}

void Esp8266HardwareAdapter::deactivateHeating() {
    Serial.println("Hardware: Deactivating heating system");
    // TODO: Implementar control de calefacción
    // digitalWrite(HEATING_PIN, LOW);
}

void Esp8266HardwareAdapter::activateCooling() {
    Serial.println("Hardware: Activating cooling system");
    // TODO: Implementar control de refrigeración
    // digitalWrite(COOLING_PIN, HIGH);
}

void Esp8266HardwareAdapter::deactivateCooling() {
    Serial.println("Hardware: Deactivating cooling system");
    // TODO: Implementar control de refrigeración
    // digitalWrite(COOLING_PIN, LOW);
}

void Esp8266HardwareAdapter::activateEmergency() {
    Serial.println("Hardware: Activating emergency system");
    // TODO: Implementar sistema de emergencia
    // digitalWrite(EMERGENCY_PIN, HIGH);
}

void Esp8266HardwareAdapter::deactivateEmergency() {
    Serial.println("Hardware: Deactivating emergency system");
    // TODO: Implementar sistema de emergencia
    // digitalWrite(EMERGENCY_PIN, LOW);
}

void Esp8266HardwareAdapter::toggleLeds() {
    ledStatus = !ledStatus; // Toggle LED status
    updateLeds(ledStatus);
} 