#ifndef I_HARDWARE_REPOSITORY_H
#define I_HARDWARE_REPOSITORY_H

#include <Arduino.h>

/**
 * @brief Interfaz para el repositorio de hardware
 * 
 * Define el contrato para manejar el hardware del sistema.
 * Siguiendo la arquitectura hexagonal, esta es la interfaz que define el puerto
 * que la aplicación espera para controlar el hardware.
 */
class IHardwareRepository {
public:
    virtual ~IHardwareRepository() = default;
    
    /**
     * @brief Inicializa el hardware del sistema
     */
    virtual void initialize() = 0;
    
    /**
     * @brief Actualiza el estado de los LEDs
     * @param status Estado de los LEDs (HIGH/LOW)
     */
    virtual void updateLeds(bool status) = 0;
    
    /**
     * @brief Ejecuta un delay con actualización de LEDs
     * @param milliseconds Tiempo de espera en milisegundos
     */
    virtual void delayWithLedUpdate(unsigned long milliseconds) = 0;
    
    /**
     * @brief Activa el sistema de calefacción
     */
    virtual void activateHeating() = 0;
    
    /**
     * @brief Desactiva el sistema de calefacción
     */
    virtual void deactivateHeating() = 0;
    
    /**
     * @brief Activa el sistema de refrigeración
     */
    virtual void activateCooling() = 0;
    
    /**
     * @brief Desactiva el sistema de refrigeración
     */
    virtual void deactivateCooling() = 0;
    
    /**
     * @brief Activa el sistema de emergencia
     */
    virtual void activateEmergency() = 0;
    
    /**
     * @brief Desactiva el sistema de emergencia
     */
    virtual void deactivateEmergency() = 0;
};

#endif // I_HARDWARE_REPOSITORY_H 