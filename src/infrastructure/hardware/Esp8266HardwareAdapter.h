#ifndef ESP8266_HARDWARE_ADAPTER_H
#define ESP8266_HARDWARE_ADAPTER_H

#include "../../domain/repositories/IHardwareRepository.h"
#include "../config.h"
#include <Arduino.h>

/**
 * @brief Adaptador para el hardware del ESP8266
 * 
 * Implementa la interfaz IHardwareRepository para controlar
 * el hardware específico del ESP8266.
 */
class Esp8266HardwareAdapter : public IHardwareRepository {
private:
    bool ledStatus;
    
public:
    Esp8266HardwareAdapter() : ledStatus(HIGH) {}
    ~Esp8266HardwareAdapter() override = default;
    
    /**
     * @brief Inicializa el hardware del ESP8266
     */
    void initialize() override;
    
    /**
     * @brief Actualiza el estado de los LEDs
     * @param status Estado de los LEDs (HIGH/LOW)
     */
    void updateLeds(bool status) override;
    
    /**
     * @brief Ejecuta un delay con actualización de LEDs
     * @param milliseconds Tiempo de espera en milisegundos
     */
    void delayWithLedUpdate(unsigned long milliseconds) override;
    
    /**
     * @brief Activa el sistema de calefacción
     */
    void activateHeating() override;
    
    /**
     * @brief Desactiva el sistema de calefacción
     */
    void deactivateHeating() override;
    
    /**
     * @brief Activa el sistema de refrigeración
     */
    void activateCooling() override;
    
    /**
     * @brief Desactiva el sistema de refrigeración
     */
    void deactivateCooling() override;
    
    /**
     * @brief Activa el sistema de emergencia
     */
    void activateEmergency() override;
    
    /**
     * @brief Desactiva el sistema de emergencia
     */
    void deactivateEmergency() override;
    
private:
    /**
     * @brief Toggle del estado de los LEDs
     */
    void toggleLeds();
};

#endif // ESP8266_HARDWARE_ADAPTER_H 