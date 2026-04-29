# Configuracion PlatformIO

## Estado actual (verificado y funcionando)

### platformio.ini

```ini
[env:esp12f]
platform = espressif8266
board = esp12e
framework = arduino
monitor_speed = 115200
build_flags =
    -I src/domain/entities
    -I src/domain/repositories
    -I src/domain/services
    -I src/application/use_cases
    -I src/application/services
    -I src/infrastructure
    -I src/infrastructure/http
    -I src/infrastructure/json
    -I src/infrastructure/hardware
    -I src/interfaces/controllers
lib_deps =
    arduino-libraries/Arduino_JSON@^0.2.0
```

## Descripcion de cada campo

| Campo | Valor | Descripcion |
|---|---|---|
| `platform` | `espressif8266` | Familia de chips Espressif para ESP8266 |
| `board` | `esp12e` | ESP-12F (PlatformIO no tiene ID propio para ESP-12F; usa `esp12e` que es identico en arquitectura) |
| `framework` | `arduino` | Framework Arduino |
| `monitor_speed` | `115200` | Baud rate del monitor serie |

## Build flags

Los `-I` agregan directorios al include path del compilador, permitiendo usar `#include "archivo.h"` sin rutas relativas largas. Corresponden a la arquitectura hexagonal del proyecto:

| Flag | Capa |
|---|---|
| `src/domain/entities` | Entidades de dominio |
| `src/domain/repositories` | Interfaces de repositorios |
| `src/domain/services` | Servicios de dominio |
| `src/application/use_cases` | Casos de uso |
| `src/application/services` | Servicios de aplicacion |
| `src/infrastructure` | Adaptadores de infraestructura |
| `src/infrastructure/http` | Cliente HTTP |
| `src/infrastructure/json` | Parser JSON |
| `src/infrastructure/hardware` | Control de pines |
| `src/interfaces/controllers` | Controlador principal |

## Dependencias

| Libreria | Version | Uso |
|---|---|---|
| `arduino-libraries/Arduino_JSON` | `^0.2.0` | Parseo de respuestas JSON de la API |

## Comandos utiles

```bash
# Compilar
pio run

# Subir al hardware
pio run --target upload

# Subir especificando puerto
pio run --target upload --upload-port /dev/ttyUSB0

# Monitor serie (115200 baud)
pio device monitor

# Limpiar build
pio run --target clean
```

## Hardware objetivo

- **Modulo:** ESP-12F (ESP8266, equivalente a ESP-12E en PlatformIO)
- **Jumper negro en pin RST:** para activar reset manualmente (conecta RST a GND)
- **Jumper azul en pin GPIO0:** para entrar en modo bootloader (conecta GPIO0 a GND)
- **Puerto tipico Linux:** `/dev/ttyUSB0`
- **Puerto tipico Windows:** `COM3` (puede variar)

### Procedimiento de upload manual (modo flash)

El ESP-12F no tiene circuito de auto-reset, por lo que hay que entrar en modo bootloader manualmente antes de cada upload:

1. Conectar jumper azul (GPIO0 a GND)
2. Presionar jumper negro (RST a GND) y soltarlo — el chip reinicia en modo flash
3. Ejecutar `pio run --target upload`
4. Al terminar el upload, desconectar jumper azul (GPIO0)
5. Presionar jumper negro (RST) nuevamente para reiniciar en modo normal
