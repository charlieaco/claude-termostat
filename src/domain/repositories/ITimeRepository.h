#ifndef I_TIME_REPOSITORY_H
#define I_TIME_REPOSITORY_H

#include <Arduino.h>

/**
 * @brief Interfaz para el repositorio de tiempo
 * 
 * Define el contrato para obtener la hora actual desde fuentes externas.
 * Siguiendo la arquitectura hexagonal, esta es la interfaz que define el puerto
 * que la aplicación espera para obtener datos de tiempo.
 */
class ITimeRepository {
public:
    virtual ~ITimeRepository() = default;
    
    /**
     * @brief Obtiene la hora actual desde la fuente de datos
     * @param currentTime Hora actual obtenida (parámetro de salida)
     * @return true si se obtuvo la hora exitosamente, false en caso contrario
     */
    virtual bool getCurrentTime(String& currentTime) = 0;
};

#endif // I_TIME_REPOSITORY_H 