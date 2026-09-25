# Dirgamochi-C3

A new, independent DasaiMochi-style firmware for an ESP32-C3 Super Mini.

This project is **not a Xiaozhi/Chronos hybrid**. Chronos is used as the phone companion/BLE data channel, while Dirgamochi owns the OLED face, buttons, navigation screen, notifications, weather, clock, music controls, phone-finder, and future audio/AI modules.

## Current hardware

### OLED
- SSD1306 128x64
- SDA = GPIO21
- SCL = GPIO20
- expected address = 0x3C

### INMP441
- SCK = GPIO1
- WS = GPIO2
- SD = GPIO8
- L/R = GND

### MAX98357A
- BCLK = GPIO1
- LRC = GPIO2
- DIN = GPIO5

### TTP223
- TALK = GPIO4
- NEXT = GPIO6
- MODE = GPIO7
- VCC = 3.3V
- GND = common GND

## Chronos features planned in this base

The ChronosESP32 library supports:
- time synchronization
- notifications
- weather
- phone battery
- navigation data and 48x48 navigation icons
- alarms
- contacts / SOS data
- music metadata
- music controls
- find-phone command
- camera command
- QR links

Dirgamochi currently implements the display-side pieces that make sense for a 128x64 OLED and the three physical TTP223 inputs.

## UI

The face is intentionally original:
- two large round/circular eyes
- blinking
- looking left/right
- happy/sleepy/alert expressions
- connection indicator
- notification and navigation pages
- no rectangular DasaiMochi eyes

## Buttons

### TALK / GPIO4
Short press opens the phone/status page. Long press sends Chronos **Find Phone**.

The INMP441/MAX98357A hardware is isolated in `audio_engine.*`; the stable voice-recording/TTS layer is the next milestone so audio bugs cannot destabilize Chronos BLE.

### NEXT / GPIO6
Cycles through:
FACE -> TIME -> WEATHER -> NOTIFICATIONS -> NAVIGATION -> MUSIC -> PHONE

### MODE / GPIO7
Short press returns to FACE.
Long press sends `MUSIC_TOGGLE`.

## Build on Android

This repository is designed for GitHub Actions so it can be built without a PC.

The workflow:
1. checkout
2. install PlatformIO
3. compile
4. package `.pio/build/esp32-c3-supermini/firmware.bin`
5. upload a ZIP artifact

## Chronos pairing

Pair the device from the Chronos app:
Chronos -> Watches -> Watches -> Pair New Devices -> Search.

Do not pair it from Android's normal Bluetooth settings.

## Important

GPIO1/GPIO2 are used by both I2S clock lines. GPIO8 is an I2S data input. GPIO20/21 are the OLED I2C pins. The code keeps this mapping centralized in `include/dirgamochi_config.h`.

The first milestone is a stable Chronos + OLED firmware. Voice AI, audio streaming and richer animations are separate modules so one broken feature cannot take down the whole device.
