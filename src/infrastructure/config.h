#ifndef CONFIG_H
#define CONFIG_H

#include "credentials.h"

// Hardware Configuration
#define LED_PIN_1 4
#define LED_PIN_2 5
#define SERIAL_BAUD_RATE 115200

// Timing Configuration
#define ONE_SECOND 1000
#define SETUP_DELAY_SECONDS 4
#define WIFI_RETRY_DELAY_SECONDS 10
#define STATE_MACHINE_DELAY_SECONDS 30
#define TOKEN_RETRY_DELAY_SECONDS 120
#define TEMPERATURE_UPDATE_DELAY_SECONDS 120

// API Configuration
#define METEO_API_HOST "api.meteomatics.com"
#define METEO_LOGIN_HOST "login.meteomatics.com"
#define METEO_API_PORT 443
#define METEO_LOGIN_PORT 443

// SSL Configuration
extern const char FINGERPRINT_SNI_CLOUDFLARESSL_COM[];

// Location Configuration (Buenos Aires, Argentina)
#define LOCATION_LATITUDE "-34.396944"
#define LOCATION_LONGITUDE "-58.694444"

// Time API Configuration
#define TIME_API_URL "http://worldtimeapi.org/api/timezone/America/Argentina/Salta"

// Temperature Thresholds
#define TEMPERATURE_HIGH_THRESHOLD 16.0
#define TEMPERATURE_MEDIUM_THRESHOLD 10.0
#define TEMPERATURE_LOW_THRESHOLD 5.0

#endif // CONFIG_H 