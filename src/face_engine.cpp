#include "face_engine.h"
#include "dirgamochi_config.h"

FaceEngine::FaceEngine(Adafruit_SSD1306& display) : d(display) {}

void FaceEngine::begin() {
    randomSeed(analogRead(0) ^ micros());
    nextBlink = millis() + random(1200, 3200);
}

void FaceEngine::drawEye(int cx, int cy, int radius, int pupilX, int pupilY) {
    d.fillCircle(cx, cy, radius, SSD1306_WHITE);
    d.fillCircle(cx + pupilX, cy + pupilY, radius / 2, SSD1306_BLACK);
    d.fillCircle(cx + pupilX - 2, cy + pupilY - 2, 2, SSD1306_WHITE);
}

void FaceEngine::update(bool connected, bool hasNotification, bool navActive) {
    uint32_t now = millis();
    if (now - lastFrame < DG_EYE_FRAME_MS) return;
    lastFrame = now;

    if (now >= nextBlink) {
        blinkUntil = now + 130;
        nextBlink = now + random(1400, 4200);
    }

    d.clearDisplay();

    bool blinking = now < blinkUntil;
    if (blinking) {
        d.drawFastHLine(17, 32, 34, SSD1306_WHITE);
        d.drawFastHLine(77, 32, 34, SSD1306_WHITE);
    } else {
        int drift = (int)((now / 500) % 3) - 1;
        int pupilY = expression == 1 ? -4 : (expression == 2 ? 3 : 0);
        drawEye(34, 32, 20, drift + look, pupilY);
        drawEye(94, 32, 20, drift + look, pupilY);

        if (expression == 3) {
            d.drawFastHLine(14, 12, 32, SSD1306_WHITE);
            d.drawFastHLine(82, 12, 32, SSD1306_WHITE);
        }
    }

    // Tiny status marks, intentionally unobtrusive.
    if (connected) d.fillCircle(4, 4, 2, SSD1306_WHITE);
    if (hasNotification) {
        d.fillCircle(124, 4, 3, SSD1306_WHITE);
    }
    if (navActive) {
        d.drawRect(58, 0, 12, 8, SSD1306_WHITE);
        d.drawPixel(63, 2, SSD1306_BLACK);
        d.drawPixel(62, 3, SSD1306_BLACK);
        d.drawPixel(64, 3, SSD1306_BLACK);
    }

    d.display();
}
void FaceEngine::setExpression(uint8_t e) {
    expression = e;
}
