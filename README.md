# Termostato Inteligente ESP8266

Sistema de control de temperatura automático basado en **ESP8266** que obtiene datos meteorológicos reales desde internet para activar calefacción o refrigeración, sin necesidad de sensores físicos de temperatura.

## ¿Cómo funciona?

En lugar de medir la temperatura con un sensor local, el dispositivo consulta la **API de Meteomatics** para obtener la temperatura exterior en tiempo real (configurado para Buenos Aires, Argentina). Según el valor obtenido, activa el hardware correspondiente.

```
WiFi → Autenticación API → Hora actual → Temperatura → Evaluar → Actuar → Esperar → (repetir)
```

## Rangos de temperatura y acciones

| Temperatura | Acción |
|---|---|
| < 5°C | Calefacción de emergencia |
| 5 – 10°C | Calefacción normal |
| 10 – 16°C | Sin acción (temperatura confortable) |
| > 16°C | Refrigeración |

## Hardware requerido

- ESP8266 (NodeMCU v2 o Wemos D1 Mini)
- Relés o LEDs en pines **4** y **5**
- Conexión WiFi

## Arquitectura

El proyecto implementa **arquitectura hexagonal (Ports and Adapters)** con 4 capas:

```
src/
├── domain/          → Lógica de negocio pura (entidades, interfaces, servicios)
├── application/     → Casos de uso y máquina de estados
├── infrastructure/  → Adaptadores: WiFi, HTTP, JSON, hardware
└── interfaces/      → Controlador principal
```

## APIs utilizadas

- [Meteomatics](https://www.meteomatics.com) — temperatura en tiempo real (requiere cuenta)
- [WorldTimeAPI](https://worldtimeapi.org) — sincronización horaria

## Instalación

### Requisitos
- [PlatformIO](https://platformio.org) (CLI o extensión de VS Code)

### Configuración de credenciales

Copia el archivo de ejemplo y completa tus datos:

```bash
cp src/infrastructure/credentials.h.example src/infrastructure/credentials.h
```

Edita `credentials.h` con tus credenciales:

```cpp
#define WIFI_SSID              "tu_red_wifi"
#define WIFI_PASSWORD          "tu_password_wifi"
#define METEO_USERNAME         "tu_usuario_meteomatics"
#define METEO_PASSWORD         "tu_password_meteomatics"
#define METEO_LOGIN_CREDENTIALS "base64(usuario:password)"
```

> El archivo `credentials.h` está excluido de git. Nunca subas tus credenciales al repositorio.

### Compilar y subir

```bash
# Compilar
pio run

# Subir al hardware
pio run --target upload --upload-port /dev/ttyUSB0

# Monitor serial
pio device monitor
```

## Configuración

Los parámetros del sistema se encuentran en `src/infrastructure/config.h`:

```cpp
#define LOCATION_LATITUDE  "-34.396944"   // Buenos Aires
#define LOCATION_LONGITUDE "-58.694444"

#define TEMPERATURE_HIGH_THRESHOLD    16.0
#define TEMPERATURE_MEDIUM_THRESHOLD  10.0
#define TEMPERATURE_LOW_THRESHOLD      5.0
```

## Licencia

MIT
