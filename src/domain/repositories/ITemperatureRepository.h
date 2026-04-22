#ifndef I_TEMPERATURE_REPOSITORY_H
#define I_TEMPERATURE_REPOSITORY_H

#include <Arduino.h>

/**
 * @brief Interfaz para el repositorio de temperatura
 * 
 * Define el contrato para obtener datos de temperatura desde fuentes externas.
 * Siguiendo la arquitectura hexagonal, esta es la interfaz que define el puerto
 * que la aplicación espera para obtener datos de temperatura.
 */
class ITemperatureRepository {
public:
    virtual ~ITemperatureRepository() = default;
    
    /**
     * @brief Obtiene la temperatura actual desde la fuente de datos
     * @param timestamp Timestamp para la consulta de temperatura
     * @param authToken Token de autenticación
     * @param temperature Temperatura obtenida (parámetro de salida)
     * @return true si se obtuvo la temperatura exitosamente, false en caso contrario
     */
    virtual bool getCurrentTemperature(const String& timestamp, const String& authToken, double& temperature) = 0;
    
    /**
     * @brief Obtiene el token de autenticación
     * @param token Token obtenido (parámetro de salida)
     * @return true si se obtuvo el token exitosamente, false en caso contrario
     */
    virtual bool getAuthToken(String& token) = 0;
};

#endif // I_TEMPERATURE_REPOSITORY_H 