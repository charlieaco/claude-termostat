#include "Esp8266HardwareAdapter.h"

void Esp8266HardwareAdapter::initialize() {
    pinMode(LED_PIN_1, OUTPUT);
    pinMode(LED_PIN_2, OUTPUT);
    digitalWrite(LED_PIN_1, LOW);
    digitalWrite(LED_PIN_2, LOW);
    Serial.println("ESP8266 hardware initialized successfully");
}

void Esp8266HardwareAdapter::updateLeds(bool status) {
    digitalWrite(LED_PIN_1, status);
    digitalWrite(LED_PIN_2, status);
}

void Esp8266HardwareAdapter::sleep(unsigned long milliseconds) {
    unsigned long startTime = millis();
    while (millis() - startTime < milliseconds) {
        delay(500);
    }
}

void Esp8266HardwareAdapter::activateHeating() {
    Serial.println("Hardware: Activating heating system");
}

void Esp8266HardwareAdapter::deactivateHeating() {
    Serial.println("Hardware: Deactivating heating system");
}

void Esp8266HardwareAdapter::activateCooling() {
    Serial.println("Hardware: Activating cooling system");
}

void Esp8266HardwareAdapter::deactivateCooling() {
    Serial.println("Hardware: Deactivating cooling system");
}

void Esp8266HardwareAdapter::activateEmergency() {
    Serial.println("Hardware: Activating emergency system");
}

void Esp8266HardwareAdapter::deactivateEmergency() {
    Serial.println("Hardware: Deactivating emergency system");
}
