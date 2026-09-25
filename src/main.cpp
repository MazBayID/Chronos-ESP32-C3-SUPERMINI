#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ChronosESP32.h>

#include "dirgamochi_config.h"
#include "face_engine.h"
#include "chronos_ui.h"
#include "audio_engine.h"

Adafruit_SSD1306 display(DG_OLED_WIDTH, DG_OLED_HEIGHT, &Wire, -1);
ChronosESP32 chronos(DG_DEVICE_NAME, CF_ESP32_240x240);

FaceEngine face(display);
ChronosUI ui(display, chronos);
AudioEngine audio;

volatile bool bleConnected = false;
volatile bool newNotification = false;
volatile bool navigationChanged = false;

struct Button {
    uint8_t pin;
    bool last = false;
    uint32_t pressedAt = 0;
    bool longSent = false;

    // Tambahan Constructor untuk memperbaiki error inisialisasi pin
    Button(uint8_t p) : pin(p) {}

    void begin() {
        pinMode(pin, INPUT); // Pastikan ini INPUT atau INPUT_PULLUP sesuai hardware-mu
        last = digitalRead(pin);
    }

    bool shortPress() {
        bool now = digitalRead(pin);
        bool event = false;

        if (now && !last) {
            pressedAt = millis();
            longSent = false;
        }

        if (!now && last) {
            uint32_t held = millis() - pressedAt;
            if (held >= DG_BUTTON_DEBOUNCE_MS && held < DG_LONG_PRESS_MS) {
                event = true;
            }
        }

        last = now;
        return event;
    }

    bool longPress() {
        bool now = digitalRead(pin);
        bool event = false;

        if (now && !last) {
            pressedAt = millis();
            longSent = false;
        }

        if (now && !longSent && millis() - pressedAt >= DG_LONG_PRESS_MS) {
            longSent = true;
            event = true;
        }

        last = now;
        return event;
    }
};

Button talkBtn{DG_TOUCH_TALK};
Button nextBtn{DG_TOUCH_NEXT};
Button modeBtn{DG_TOUCH_MODE};

void connectionCallback(bool state) {
    bleConnected = state;
    Serial.printf("[Chronos] %s\n", state ? "CONNECTED" : "DISCONNECTED");
}

void notificationCallback(Notification n) {
    newNotification = true;
    Serial.printf("[Notif] %s | %s | %s\n",
                  n.app.c_str(), n.title.c_str(), n.message.c_str());
    ui.setPage(UiPage::NOTIFICATIONS);
}

void ringerCallback(String caller, bool state) {
    Serial.printf("[Ringer] %s: %s\n", state ? "INCOMING" : "ENDED", caller.c_str());
    if (state) ui.setPage(UiPage::PHONE);
}

void configCallback(Config config, uint32_t a, uint32_t b) {
    switch (config) {
        case CF_TIME:
            Serial.println("[Chronos] time sync");
            break;
        case CF_WEATHER:
            Serial.println("[Chronos] weather update");
            break;
        case CF_PBAT:
            Serial.printf("[Chronos] phone battery=%u%% charging=%u\n",
                          (unsigned)b, (unsigned)a);
            break;
        case CF_NAV_DATA:
            navigationChanged = true;
            Serial.printf("[Chronos] navigation active=%u\n", (unsigned)a);
            break;
        case CF_NAV_ICON:
            navigationChanged = true;
            break;
        case CF_MUSIC:
            Serial.println("[Chronos] music update");
            break;
        case CF_ALARM:
            Serial.println("[Chronos] alarm update");
            break;
        case CF_CONTACT:
            Serial.println("[Chronos] contact update");
            break;
        default:
            break;
    }
}

void setup() {
    Serial.begin(115200);
    delay(250);

    Serial.println();
    Serial.println("=================================");
    Serial.println("      DIRGAMOCHI-C3 v0.1");
    Serial.println("  Chronos + Cute Round Eyes");
    Serial.println("=================================");

    talkBtn.begin();
    nextBtn.begin();
    modeBtn.begin();

    Wire.begin(DG_OLED_SDA, DG_OLED_SCL);

    if (!display.begin(SSD1306_SWITCHCAPVCC, DG_OLED_ADDR)) {
        Serial.println("[OLED] ERROR: SSD1306 not found");
        while (true) {
            delay(1000);
        }
    }

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(28, 20);
    display.print("DIRGAMOCHI");
    display.setCursor(30, 36);
    display.print("starting...");
    display.display();

    face.begin();
    ui.begin();
    audio.begin();

    chronos.setConnectionCallback(connectionCallback);
    chronos.setNotificationCallback(notificationCallback);
    chronos.setRingerCallback(ringerCallback);
    chronos.setConfigurationCallback(configCallback);
    chronos.setName(DG_DEVICE_NAME);
    chronos.setScreen(CF_ESP32_240x240);
    chronos.setChunkedTransfer(true);

    chronos.begin();
    chronos.setBattery(DG_CHRONOS_BATTERY);

    Serial.printf("[Chronos] BLE address: %s\n", chronos.getAddress().c_str());
    Serial.println("[System] Ready.");
}

void loop() {
    chronos.loop();

    if (nextBtn.shortPress()) {
        ui.nextPage();
    }

    if (modeBtn.shortPress()) {
        ui.setPage(UiPage::FACE);
    }

    if (modeBtn.longPress()) {
        chronos.musicControl(MUSIC_TOGGLE);
        ui.setPage(UiPage::MUSIC);
    }

    if (talkBtn.shortPress()) {
        // Voice interaction is intentionally isolated from the Chronos layer
        // in v0.1. This confirms the TALK input is electrically working.
        ui.setPage(UiPage::PHONE);
        Serial.println("[TALK] input detected");
    }

    if (talkBtn.longPress()) {
        // Chronos supports Find Phone; this is a useful function that needs
        // no extra hardware.
        chronos.findPhone(true);
        ui.setPage(UiPage::PHONE);
        Serial.println("[TALK] find-phone command sent");
    }

    // Face mode gets the animation; information pages get the Chronos UI.
    if (ui.getPage() == UiPage::FACE) {
        face.update(bleConnected,
                    chronos.getNotificationCount() > 0,
                    chronos.getNavigation().active);
    } else {
        ui.render(bleConnected);
    }

    delay(10);
}
