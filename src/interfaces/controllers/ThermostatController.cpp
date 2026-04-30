#include "ThermostatController.h"
#include "../../infrastructure/config.h"

void ThermostatController::initialize() {
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println("\n=== Termostato Inteligente ESP8266 (Arquitectura Hexagonal) ===");
    
    // Setup delay
    for (uint8_t t = SETUP_DELAY_SECONDS; t > 0; t--) {
        Serial.printf("Initializing in %d seconds...\n", t);
        delay(ONE_SECOND);
    }
    
    // Inicializar hardware
    hardwareAdapter.initialize();
    
    // Inicializar WiFi
    if (wifiRepository.initialize()) {
        Serial.println("WiFi initialized successfully");
    } else {
        Serial.println("WiFi initialization failed");
    }
    
    Serial.println("Thermostat controller initialized successfully");
    transitionTo(ThermostatState::INITIALIZING);
}

void ThermostatController::run() {
    if (!wifiRepository.isConnected()) {
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

void ThermostatController::handleIdleState() {
    Serial.println("Controller State: IDLE - Waiting for WiFi connection");
    if (wifiRepository.isConnected()) {
        transitionTo(ThermostatState::GETTING_AUTH_TOKEN);
    }
}

void ThermostatController::handleInitializingState() {
    Serial.println("Controller State: INITIALIZING - Setting up system");
    transitionTo(ThermostatState::IDLE);
}

void ThermostatController::handleGettingAuthTokenState() {
    Serial.println("Controller State: GETTING_AUTH_TOKEN - Obtaining authentication token");
    transitionTo(ThermostatState::GETTING_CURRENT_TIME);
}

void ThermostatController::handleGettingCurrentTimeState() {
    Serial.println("Controller State: GETTING_CURRENT_TIME - Obtaining current time");
    transitionTo(ThermostatState::GETTING_TEMPERATURE);
}

void ThermostatController::handleGettingTemperatureState() {
    Serial.println("Controller State: GETTING_TEMPERATURE - Obtaining temperature data");
    
    if (getTemperatureUseCase.execute(currentTemperature)) {
        Serial.printf("Controller Temperature obtained: %.2f°C\n", currentTemperature);
        transitionTo(ThermostatState::EVALUATING_DECISION);
    } else {
        Serial.println("Controller Failed to get temperature - retrying");
        transitionTo(ThermostatState::GETTING_AUTH_TOKEN);
    }
}

void ThermostatController::handleEvaluatingDecisionState() {
    Serial.println("Controller State: EVALUATING_DECISION - Evaluating temperature and taking action");
    
    if (controlTemperatureUseCase.execute(currentTemperature)) {
        Serial.println("Controller Temperature control executed successfully");
    } else {
        Serial.println("Controller Temperature control failed");
    }
    
    transitionTo(ThermostatState::WAITING);
}

void ThermostatController::handleWaitingState() {
    Serial.println("Controller State: WAITING - Waiting before next cycle");
    delayWithLedUpdate(TEMPERATURE_UPDATE_DELAY_SECONDS * ONE_SECOND);
    transitionTo(ThermostatState::GETTING_CURRENT_TIME);
}

void ThermostatController::transitionTo(ThermostatState newState) {
    Serial.printf("Controller Transitioning from %d to %d\n", static_cast<int>(currentState), static_cast<int>(newState));
    currentState = newState;
}

void ThermostatController::delayWithLedUpdate(unsigned long milliseconds) {
    hardwareAdapter.delayWithLedUpdate(milliseconds);
} 