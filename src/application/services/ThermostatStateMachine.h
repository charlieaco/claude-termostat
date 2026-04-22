#ifndef THERMOSTAT_STATE_MACHINE_H
#define THERMOSTAT_STATE_MACHINE_H

#include "ThermostatState.h"
#include "HttpClient.h"
#include "JsonParser.h"
#include "TemperatureController.h"
#include <ESP8266WiFiMulti.h>
#include <Arduino.h>

class ThermostatStateMachine {
public:
    ThermostatStateMachine();
    
    void initialize();
    void run();
    bool isConnected() const;
    
private:
    ThermostatState currentState;
    mutable ESP8266WiFiMulti wifiMulti;
    
    // State data
    String authToken;
    String currentTime;
    double currentTemperature;
    
    // State handlers
    void handleIdleState();
    void handleInitializingState();
    void handleGettingAuthTokenState();
    void handleGettingCurrentTimeState();
    void handleGettingTemperatureState();
    void handleEvaluatingDecisionState();
    void handleWaitingState();
    
    // Helper methods
    void transitionTo(ThermostatState newState);
    void updateLeds();
    void delayWithLedUpdate(unsigned long milliseconds);
};

#endif // THERMOSTAT_STATE_MACHINE_H 