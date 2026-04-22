#ifndef GET_TEMPERATURE_USE_CASE_H
#define GET_TEMPERATURE_USE_CASE_H

#include "../../domain/repositories/ITemperatureRepository.h"
#include "../../domain/repositories/ITimeRepository.h"
#include <Arduino.h>

/**
 * @brief Caso de uso para obtener la temperatura actual
 * 
 * Encapsula la lógica de negocio para obtener la temperatura actual,
 * incluyendo la obtención del token de autenticación y la hora actual.
 */
class GetTemperatureUseCase {
private:
    ITemperatureRepository& temperatureRepository;
    ITimeRepository& timeRepository;
    
public:
    GetTemperatureUseCase(ITemperatureRepository& tempRepo, ITimeRepository& timeRepo)
        : temperatureRepository(tempRepo), timeRepository(timeRepo) {}
    
    /**
     * @brief Ejecuta el caso de uso para obtener la temperatura
     * @param temperature Temperatura obtenida (parámetro de salida)
     * @return true si se obtuvo la temperatura exitosamente, false en caso contrario
     */
    bool execute(double& temperature);
    
private:
    /**
     * @brief Obtiene el token de autenticación
     * @param token Token obtenido (parámetro de salida)
     * @return true si se obtuvo el token exitosamente, false en caso contrario
     */
    bool getAuthToken(String& token);
    
    /**
     * @brief Obtiene la hora actual
     * @param currentTime Hora actual obtenida (parámetro de salida)
     * @return true si se obtuvo la hora exitosamente, false en caso contrario
     */
    bool getCurrentTime(String& currentTime);
};

#endif // GET_TEMPERATURE_USE_CASE_H 