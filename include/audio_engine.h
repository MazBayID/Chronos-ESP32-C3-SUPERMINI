#pragma once

#include <Arduino.h>

// Hardware audio is intentionally isolated from Chronos BLE.
// This module is the next layer for stable INMP441/MAX98357A
// voice interaction. It currently provides a hardware-safe diagnostic API.

class AudioEngine {
public:
    void begin();
    void stop();

    // Returns a short diagnostic RMS-like level from the INMP441.
    // This is non-blocking and can be expanded into recording/TTS later.
    uint32_t sampleLevel();
};
