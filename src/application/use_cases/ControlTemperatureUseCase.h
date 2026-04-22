#ifndef CONTROL_TEMPERATURE_USE_CASE_H
#define CONTROL_TEMPERATURE_USE_CASE_H

#include "../../domain/services/TemperatureController.h"
#include "../../domain/repositories/IHardwareRepository.h"
#include <Arduino.h>

/**
 * @brief Caso de uso para controlar la temperatura
 * 
 * Encapsula la lógica de negocio para evaluar la temperatura y
 * ejecutar las acciones de control correspondientes.
 */
class ControlTemperatureUseCase {
private:
    IHardwareRepository& hardwareRepository;
    
public:
    ControlTemperatureUseCase(IHardwareRepository& hwRepo)
        : hardwareRepository(hwRepo) {}
    
    /**
     * @brief Ejecuta el caso de uso para controlar la temperatura
     * @param temperature Temperatura actual a evaluar
     * @return true si se ejecutó el control exitosamente, false en caso contrario
     */
    bool execute(double temperature);
    
private:
    /**
     * @brief Ejecuta la acción correspondiente al rango de temperatura
     * @param range Rango de temperatura evaluado
     * @param temperature Temperatura actual
     */
    void executeTemperatureAction(TemperatureController::TemperatureRange range, double temperature);
};

#endif // CONTROL_TEMPERATURE_USE_CASE_H 