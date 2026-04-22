#ifndef METEO_API_ADAPTER_H
#define METEO_API_ADAPTER_H

#include "../../domain/repositories/ITemperatureRepository.h"
#include "HttpClient.h"
#include "JsonParser.h"
#include "../config.h"
#include <Arduino.h>

/**
 * @brief Adaptador para la API de Meteomatics
 * 
 * Implementa la interfaz ITemperatureRepository para obtener datos
 * de temperatura desde la API de Meteomatics.
 */
class MeteoApiAdapter : public ITemperatureRepository {
public:
    MeteoApiAdapter() = default;
    ~MeteoApiAdapter() override = default;
    
    /**
     * @brief Obtiene la temperatura actual desde Meteomatics API
     * @param timestamp Timestamp para la consulta de temperatura
     * @param authToken Token de autenticación
     * @param temperature Temperatura obtenida (parámetro de salida)
     * @return true si se obtuvo la temperatura exitosamente, false en caso contrario
     */
    bool getCurrentTemperature(const String& timestamp, const String& authToken, double& temperature) override;
    
    /**
     * @brief Obtiene el token de autenticación de Meteomatics
     * @param token Token obtenido (parámetro de salida)
     * @return true si se obtuvo el token exitosamente, false en caso contrario
     */
    bool getAuthToken(String& token) override;
    
private:
    /**
     * @brief Construye la URL para obtener datos de temperatura
     * @param timestamp Timestamp para la consulta
     * @param token Token de autenticación
     * @return URL construida
     */
    String buildTemperatureUrl(const String& timestamp, const String& token);
};

#endif // METEO_API_ADAPTER_H 