#include "chronos_ui.h"

ChronosUI::ChronosUI(Adafruit_SSD1306& display, ChronosESP32& chronos)
    : d(display), c(chronos) {}

void ChronosUI::begin() {
    page = UiPage::FACE;
}

void ChronosUI::nextPage() {
    page = static_cast<UiPage>((static_cast<uint8_t>(page) + 1) % 7);
}

void ChronosUI::setPage(UiPage p) {
    page = p;
}

void ChronosUI::title(const char* text) {
    d.setTextSize(1);
    d.setTextColor(SSD1306_WHITE);
    d.setCursor(0, 0);
    d.print(text);
    d.drawFastHLine(0, 9, 128, SSD1306_WHITE);
}

void ChronosUI::textWrap(const String& s, int y, int maxChars) {
    d.setTextSize(1);
    d.setTextColor(SSD1306_WHITE);

    int line = 0;
    String word;
    String current;
    for (size_t i = 0; i <= s.length(); ++i) {
        char ch = (i < s.length()) ? s[i] : ' ';
        if (ch == ' ' || ch == '\n') {
            if (current.length() + word.length() + 1 > (size_t)maxChars) {
                d.setCursor(0, y + line * 10);
                d.print(current);
                current = word;
                line++;
            } else {
                if (current.length()) current += ' ';
                current += word;
            }
            word = "";
            if (ch == '\n') {
                d.setCursor(0, y + line * 10);
                d.print(current);
                current = "";
                line++;
            }
            if (line >= 5) break;
        } else {
            word += ch;
        }
    }
    if (line < 5 && current.length()) {
        d.setCursor(0, y + line * 10);
        d.print(current);
    }
}

void ChronosUI::drawNavIcon(const Navigation& nav, int ox, int oy) {
    if (!nav.hasIcon) return;
    for (int y = 0; y < 48; y++) {
        for (int x = 0; x < 48; x++) {
            int bi = (y * 48 + x) / 8;
            int bit = 7 - (x % 8);
            if ((nav.icon[bi] >> bit) & 1) {
                d.drawPixel(ox + x, oy + y, SSD1306_WHITE);
            }
        }
    }
}

void ChronosUI::render(bool connected) {
    d.clearDisplay();
    d.setTextColor(SSD1306_WHITE);

    switch (page) {
        case UiPage::FACE:
            d.setTextSize(1);
            d.setCursor(42, 56);
            d.print(connected ? "CHRONOS OK" : "OFFLINE");
            break;

        case UiPage::TIME: {
            title("TIME");
            String hm = c.getTime("%H:%M");
            d.setTextSize(3);
            int x = (128 - (hm.length() * 18)) / 2;
            if (x < 0) x = 0;
            d.setCursor(x, 20);
            d.print(hm);
            d.setTextSize(1);
            d.setCursor(34, 50);
            d.print(c.getTime("%d/%m/%Y"));
            break;
        }

        case UiPage::WEATHER: {
            title("WEATHER");
            if (c.getWeatherCount() > 0) {
                Weather& w = c.getWeatherAt(0);
                d.setTextSize(2);
                d.setCursor(0, 17);
                d.print(w.temp);
                d.print(" C");
                d.setTextSize(1);
                d.setCursor(0, 40);
                d.print(c.getWeatherCity());
                d.setCursor(0, 52);
                d.print("H:");
                d.print(w.high);
                d.print(" L:");
                d.print(w.low);
            } else {
                d.setCursor(0, 25);
                d.print("Waiting for weather...");
            }
            break;
        }

        case UiPage::NOTIFICATIONS: {
            title("NOTIFICATION");
            int n = c.getNotificationCount();
            if (n > 0) {
                Notification& x = c.getNotificationAt(0);
                d.setCursor(0, 13);
                d.print(x.app.substring(0, 20));
                d.setCursor(0, 24);
                d.print(x.title.substring(0, 20));
                textWrap(x.message, 36, 21);
            } else {
                d.setCursor(0, 28);
                d.print("No notifications");
            }
            break;
        }

        case UiPage::NAVIGATION: {
            title("NAVIGATION");
            Navigation& n = c.getNavigation();
            if (!n.active) {
                d.setCursor(0, 28);
                d.print("Navigation idle");
            } else {
                drawNavIcon(n, 0, 14);
                d.setCursor(52, 15);
                d.print(n.directions.substring(0, 12));
                d.setCursor(52, 27);
                d.print(n.distance.substring(0, 12));
                d.setCursor(52, 39);
                d.print(n.duration.substring(0, 12));
                d.setCursor(52, 51);
                d.print(n.title.substring(0, 12));
            }
            break;
        }

        case UiPage::MUSIC: {
            title("MUSIC");
            MusicInfo& m = c.getMusicInfo();
            d.setCursor(0, 18);
            d.print(m.title.substring(0, 21));
            d.setCursor(0, 31);
            d.print(m.artist.substring(0, 21));
            d.setCursor(0, 48);
            d.print("MODE: TOGGLE");
            break;
        }

        case UiPage::PHONE: {
            title("PHONE");
            d.setCursor(0, 18);
            d.print("Battery: ");
            d.print(c.getPhoneBattery());
            d.print("%");
            d.setCursor(0, 32);
            d.print(c.isPhoneCharging() ? "Charging" : "Not charging");
            d.setCursor(0, 50);
            d.print("MODE = find phone");
            break;
        }
    }

    d.display();
}
