#pragma once
#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <ChronosESP32.h>

enum class UiPage : uint8_t {
    FACE,
    TIME,
    WEATHER,
    NOTIFICATIONS,
    NAVIGATION,
    MUSIC,
    PHONE
};

class ChronosUI {
public:
    ChronosUI(Adafruit_SSD1306& display, ChronosESP32& chronos);

    void begin();
    void nextPage();
    void setPage(UiPage page);
    UiPage getPage() const { return page; }

    void render(bool connected);

private:
    Adafruit_SSD1306& d;
    ChronosESP32& c;
    UiPage page = UiPage::FACE;

    void title(const char* text);
    void textWrap(const String& s, int y, int maxChars);
    void drawNavIcon(const Navigation& nav, int ox, int oy);
};
