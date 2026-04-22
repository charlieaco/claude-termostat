#ifndef WORLD_TIME_API_ADAPTER_H
#define WORLD_TIME_API_ADAPTER_H

#include "../../domain/repositories/ITimeRepository.h"
#include "HttpClient.h"
#include "JsonParser.h"
#include "../config.h"
#include <Arduino.h>

/**
 * @brief Adaptador para la API de WorldTime
 * 
 * Implementa la interfaz ITimeRepository para obtener la hora actual
 * desde la API de WorldTime.
 */
class WorldTimeApiAdapter : public ITimeRepository {
public:
    WorldTimeApiAdapter() = default;
    ~WorldTimeApiAdapter() override = default;
    
    /**
     * @brief Obtiene la hora actual desde WorldTime API
     * @param currentTime Hora actual obtenida (parámetro de salida)
     * @return true si se obtuvo la hora exitosamente, false en caso contrario
     */
    bool getCurrentTime(String& currentTime) override;
    
private:
    /**
     * @brief Formatea el timestamp recibido de la API
     * @param rawDateTime Timestamp raw de la API
     * @return Timestamp formateado
     */
    String formatTimestamp(const String& rawDateTime);
};

#endif // WORLD_TIME_API_ADAPTER_H 