#ifndef THERMOSTAT_STATE_H
#define THERMOSTAT_STATE_H

enum class ThermostatState {
    IDLE,                    // Estado de espera inicial
    INITIALIZING,           // Inicialización del sistema
    GETTING_AUTH_TOKEN,     // Obtención del token de autenticación
    GETTING_CURRENT_TIME,   // Obtención de la hora actual
    GETTING_TEMPERATURE,    // Obtención de datos de temperatura
    EVALUATING_DECISION,    // Evaluación y toma de decisiones
    WAITING                 // Tiempo de espera entre ciclos
};

#endif // THERMOSTAT_STATE_H 