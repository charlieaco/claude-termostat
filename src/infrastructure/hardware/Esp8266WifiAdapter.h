#ifndef ESP8266_WIFI_ADAPTER_H
#define ESP8266_WIFI_ADAPTER_H

#include "../../domain/repositories/IWifiRepository.h"
#include "../config.h"
#include <ESP8266WiFiMulti.h>
#include <Arduino.h>

/**
 * @brief Adaptador para WiFi del ESP8266
 * 
 * Implementa la interfaz IWifiRepository para manejar
 * la conectividad WiFi del ESP8266.
 */
class Esp8266WifiAdapter : public IWifiRepository {
private:
    mutable ESP8266WiFiMulti wifiMulti;
    
public:
    Esp8266WifiAdapter() = default;
    ~Esp8266WifiAdapter() override = default;
    
    /**
     * @brief Inicializa la conexión WiFi
     * @return true si la inicialización fue exitosa, false en caso contrario
     */
    bool initialize() override;
    
    /**
     * @brief Verifica si está conectado a WiFi
     * @return true si está conectado, false en caso contrario
     */
    bool isConnected() const override;
    
    /**
     * @brief Ejecuta el ciclo de WiFi (mantenimiento de conexión)
     * @return true si la conexión está activa, false en caso contrario
     */
    bool run() override;
    
private:
    /**
     * @brief Configura el modo WiFi
     */
    void setupWifiMode();
    
    /**
     * @brief Agrega las redes WiFi configuradas
     */
    void addWifiNetworks();
};

#endif // ESP8266_WIFI_ADAPTER_H 