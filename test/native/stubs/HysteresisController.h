// test/native/stubs/HysteresisController.h
// STUB: no-op hasta que src/domain/services/HysteresisController.h exista (feature/add-pid).
// Los tests de test_hysteresis_controller FALLARÁN en runtime hasta entonces.
// NO definir BOILER_ENABLE_TEMP/DISABLE_TEMP aquí — vienen de config.h en la impl real.
#pragma once

class HysteresisController {
public:
    static bool evaluate(double /*temperature*/, bool currentlyEnabled) {
        return currentlyEnabled; // stub: mantiene estado sin lógica real
    }
};
