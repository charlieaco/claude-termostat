#ifndef I_WIFI_REPOSITORY_H
#define I_WIFI_REPOSITORY_H

#include <Arduino.h>

/**
 * @brief Interfaz para el repositorio de WiFi
 * 
 * Define el contrato para manejar la conexión WiFi.
 * Siguiendo la arquitectura hexagonal, esta es la interfaz que define el puerto
 * que la aplicación espera para manejar la conectividad de red.
 */
class IWifiRepository {
public:
    virtual ~IWifiRepository() = default;
    
    /**
     * @brief Inicializa la conexión WiFi
     * @return true si la inicialización fue exitosa, false en caso contrario
     */
    virtual bool initialize() = 0;
    
    /**
     * @brief Verifica si está conectado a WiFi
     * @return true si está conectado, false en caso contrario
     */
    virtual bool isConnected() const = 0;
    
    /**
     * @brief Ejecuta el ciclo de WiFi (mantenimiento de conexión)
     * @return true si la conexión está activa, false en caso contrario
     */
    virtual bool run() = 0;
};

#endif // I_WIFI_REPOSITORY_H 