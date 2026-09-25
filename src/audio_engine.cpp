#include "audio_engine.h"
#include "dirgamochi_config.h"
#include <driver/i2s.h>

static const i2s_port_t AUDIO_PORT = I2S_NUM_0;
static bool audioStarted = false;

void AudioEngine::begin() {
    // The first firmware milestone keeps the audio peripheral unclaimed
    // during Chronos operation. This prevents BLE/UI instability while
    // we build the voice layer separately.
    audioStarted = false;
}

void AudioEngine::stop() {
    if (audioStarted) {
        i2s_driver_uninstall(AUDIO_PORT);
        audioStarted = false;
    }
}

uint32_t AudioEngine::sampleLevel() {
    // Reserved for the next voice milestone.
    // Returning zero is intentional; no I2S driver is installed here yet.
    return 0;
}
