# Chronos integration reference

Dirgamochi uses the open-source `ChronosESP32` library by fbiego.

Reference:
https://github.com/fbiego/chronos-esp32

The library exposes:
- `ChronosESP32::begin()`
- `ChronosESP32::loop()`
- connection callback
- notification callback
- configuration callback
- notification storage
- weather storage
- navigation storage and 48x48 icon data
- phone battery state
- music metadata
- music controls
- find-phone command
- alarms
- contacts
- QR links

Pairing is performed from the Chronos app rather than Android's normal Bluetooth settings.

Dirgamochi intentionally keeps Chronos as a transport/data layer. The face/UI belongs to Dirgamochi.

The project does not copy the ChronosESP32 library into this repository; PlatformIO downloads it from the upstream repository during the build.
