#include "ThermostatStateMachine.h"
#include "config.h"

ThermostatStateMachine::ThermostatStateMachine() 
    : currentState(ThermostatState::IDLE), currentTemperature(0.0) {
}

void ThermostatStateMachine::initialize() {
    // Initialize hardware
    pinMode(LED_PIN_2, OUTPUT);
    pinMode(LED_PIN_2, OUTPUT);
    
    // Initialize serial communication
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println("\n=== Termostato Inteligente ESP8266 ===");
    
    // Setup delay
    for (uint8_t t = SETUP_DELAY_SECONDS; t > 0; t--) {
        Serial.printf("Initializing in %d seconds...\n", t);
        delay(ONE_SECOND);
    }
    
    // Initialize WiFi
    WiFi.mode(WIFI_STA);
    wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
    
    Serial.println("Thermostat initialized successfully");
    transitionTo(ThermostatState::INITIALIZING);
}

void ThermostatStateMachine::run() {
    if (!isConnected()) {
        Serial.println("WiFi not connected - waiting for connection...");
        delayWithLedUpdate(WIFI_RETRY_DELAY_SECONDS * ONE_SECOND);
        transitionTo(ThermostatState::INITIALIZING);
        return;
    }
    
    switch (currentState) {
        case ThermostatState::IDLE:
            handleIdleState();
            break;
        case ThermostatState::INITIALIZING:
            handleInitializingState();
            break;
        case ThermostatState::GETTING_AUTH_TOKEN:
            handleGettingAuthTokenState();
            break;
        case ThermostatState::GETTING_CURRENT_TIME:
            handleGettingCurrentTimeState();
            break;
        case ThermostatState::GETTING_TEMPERATURE:
            handleGettingTemperatureState();
            break;
        case ThermostatState::EVALUATING_DECISION:
            handleEvaluatingDecisionState();
            break;
        case ThermostatState::WAITING:
            handleWaitingState();
            break;
    }
    
    delayWithLedUpdate(STATE_MACHINE_DELAY_SECONDS * ONE_SECOND);
}

bool ThermostatStateMachine::isConnected() const {
    return wifiMulti.run() == WL_CONNECTED;
}

void ThermostatStateMachine::handleIdleState() {
    Serial.println("State: IDLE - Waiting for WiFi connection");
    if (isConnected()) {
        transitionTo(ThermostatState::GETTING_AUTH_TOKEN);
    }
}

void ThermostatStateMachine::handleInitializingState() {
    Serial.println("State: INITIALIZING - Setting up system");
    transitionTo(ThermostatState::IDLE);
}

void ThermostatStateMachine::handleGettingAuthTokenState() {
    Serial.println("State: GETTING_AUTH_TOKEN - Obtaining authentication token");
    
    if (HttpClient::getAuthToken(authToken)) {
        Serial.println("Authentication token obtained successfully");
        transitionTo(ThermostatState::GETTING_CURRENT_TIME);
    } else {
        Serial.println("Failed to get authentication token - retrying later");
        delayWithLedUpdate(TOKEN_RETRY_DELAY_SECONDS * ONE_SECOND);
    }
}

void ThermostatStateMachine::handleGettingCurrentTimeState() {
    Serial.println("State: GETTING_CURRENT_TIME - Obtaining current time");
    
    if (HttpClient::getCurrentTime(currentTime)) {
        Serial.printf("Current time: %s\n", currentTime.c_str());
        transitionTo(ThermostatState::GETTING_TEMPERATURE);
    } else {
        Serial.println("Failed to get current time - retrying");
        transitionTo(ThermostatState::GETTING_AUTH_TOKEN);
    }
}

void ThermostatStateMachine::handleGettingTemperatureState() {
    Serial.println("State: GETTING_TEMPERATURE - Obtaining temperature data");
    
    String temperatureResponse;
    if (HttpClient::getTemperatureData(currentTime, authToken, temperatureResponse)) {
        if (JsonParser::extractTemperatureValue(temperatureResponse, currentTemperature)) {
            Serial.printf("Temperature obtained: %.2f°C\n", currentTemperature);
            transitionTo(ThermostatState::EVALUATING_DECISION);
        } else {
            Serial.println("Failed to parse temperature data");
            transitionTo(ThermostatState::GETTING_AUTH_TOKEN);
        }
    } else {
        Serial.println("Failed to get temperature data - retrying");
        transitionTo(ThermostatState::GETTING_AUTH_TOKEN);
    }
}

void ThermostatStateMachine::handleEvaluatingDecisionState() {
    Serial.println("State: EVALUATING_DECISION - Evaluating temperature and taking action");
    
    auto temperatureRange = TemperatureController::evaluateTemperature(currentTemperature);
    TemperatureController::executeTemperatureAction(temperatureRange, currentTemperature);
    
    transitionTo(ThermostatState::WAITING);
}

void ThermostatStateMachine::handleWaitingState() {
    Serial.println("State: WAITING - Waiting before next cycle");
    delayWithLedUpdate(TEMPERATURE_UPDATE_DELAY_SECONDS * ONE_SECOND);
    transitionTo(ThermostatState::GETTING_CURRENT_TIME);
}

void ThermostatStateMachine::transitionTo(ThermostatState newState) {
    Serial.printf("Transitioning from %d to %d\n", static_cast<int>(currentState), static_cast<int>(newState));
    currentState = newState;
}

void ThermostatStateMachine::updateLeds() {
    static bool ledStatus = HIGH;
    ledStatus = !ledStatus; // Toggle LED status
    
    digitalWrite(LED_PIN_2, ledStatus);
    digitalWrite(LED_PIN_2, ledStatus);
}

void ThermostatStateMachine::delayWithLedUpdate(unsigned long milliseconds) {
    unsigned long startTime = millis();
    while (millis() - startTime < milliseconds) {
        updateLeds();
        delay(500); // Update LEDs every 500ms
    }
} 