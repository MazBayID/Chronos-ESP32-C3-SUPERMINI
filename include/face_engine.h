#pragma once
#include <Arduino.h>
#include <Adafruit_SSD1306.h>

class FaceEngine {
public:
    explicit FaceEngine(Adafruit_SSD1306& display);

    void begin();
    void update(bool connected, bool hasNotification, bool navActive);
    void setExpression(uint8_t expression);

private:
    Adafruit_SSD1306& d;
    uint32_t lastFrame = 0;
    uint32_t blinkUntil = 0;
    uint32_t nextBlink = 0;
    uint8_t expression = 0;
    int look = 0;

    void drawEye(int cx, int cy, int radius, int pupilX, int pupilY);
};
