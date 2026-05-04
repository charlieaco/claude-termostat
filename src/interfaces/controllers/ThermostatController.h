#ifndef THERMOSTAT_CONTROLLER_H
#define THERMOSTAT_CONTROLLER_H

#include "../../domain/entities/ThermostatState.h"
#include "../../domain/repositories/IWifiRepository.h"
#include "../../application/use_cases/GetTemperatureUseCase.h"
#include "../../application/use_cases/ControlTemperatureUseCase.h"
#include "../../infrastructure/hardware/Esp8266HardwareAdapter.h"
#include <Arduino.h>

/**
 * @brief Controlador principal del termostato
 * 
 * Orquesta todos los componentes del sistema siguiendo la arquitectura hexagonal.
 * Coordina los casos de uso, repositorios y adaptadores para mantener
 * la lógica de negocio separada de las implementaciones específicas.
 */
class ThermostatController {
private:
    // Estados
    ThermostatState currentState;
    
    // Repositorios (interfaces)
    IWifiRepository& wifiRepository;
    
    // Casos de uso
    GetTemperatureUseCase& getTemperatureUseCase;
    ControlTemperatureUseCase& controlTemperatureUseCase;
    
    // Hardware
    Esp8266HardwareAdapter& hardwareAdapter;
    
    // Datos del sistema
    double currentTemperature;
    
public:
    ThermostatController(
        IWifiRepository& wifiRepo,
        GetTemperatureUseCase& tempUseCase,
        ControlTemperatureUseCase& controlUseCase,
        Esp8266HardwareAdapter& hwAdapter
    ) : currentState(ThermostatState::IDLE),
        wifiRepository(wifiRepo),
        getTemperatureUseCase(tempUseCase),
        controlTemperatureUseCase(controlUseCase),
        hardwareAdapter(hwAdapter),
        currentTemperature(0.0) {}
    
    /**
     * @brief Inicializa el controlador del termostato
     */
    void initialize();
    
    /**
     * @brief Ejecuta el ciclo principal del termostato
     */
    void run();
    
private:
    // Manejadores de estado
    void handleIdleState();
    void handleInitializingState();
    void handleGettingAuthTokenState();
    void handleGettingCurrentTimeState();
    void handleGettingTemperatureState();
    void handleEvaluatingDecisionState();
    void handleWaitingState();
    
    // Métodos auxiliares
    void transitionTo(ThermostatState newState);
    void sleep(unsigned long milliseconds);
};

#endif // THERMOSTAT_CONTROLLER_H 