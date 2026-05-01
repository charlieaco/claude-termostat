#include "Esp8266HardwareAdapter.h"

void Esp8266HardwareAdapter::initialize() {
    pinMode(LED_PIN_1, OUTPUT);
    pinMode(LED_PIN_2, OUTPUT);
    digitalWrite(LED_PIN_1, HIGH);
    digitalWrite(LED_PIN_2, LOW);
    Serial.println("ESP8266 hardware initialized successfully");
}

void Esp8266HardwareAdapter::updateLeds(bool status) {
    digitalWrite(LED_PIN_1, status);
}

void Esp8266HardwareAdapter::sleep(unsigned long milliseconds) {
    digitalWrite(LED_PIN_1, LOW);
    delay(milliseconds);
    digitalWrite(LED_PIN_1, HIGH);
}

void Esp8266HardwareAdapter::activateHeating() {
    digitalWrite(LED_PIN_2, HIGH);
}

void Esp8266HardwareAdapter::deactivateHeating() {
    digitalWrite(LED_PIN_2, LOW);
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
