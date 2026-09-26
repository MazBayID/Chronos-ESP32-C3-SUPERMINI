#pragma once

// =========================
// Dirgamochi-C3 hardware
// =========================

// OLED
#define DG_OLED_SDA       21
#define DG_OLED_SCL       20
#define DG_OLED_ADDR      0x3C
#define DG_OLED_WIDTH     128
#define DG_OLED_HEIGHT    64

// INMP441
#define DG_MIC_BCLK       1
#define DG_MIC_WS         2
#define DG_MIC_DATA       10

// MAX98357A
#define DG_SPK_BCLK       1
#define DG_SPK_LRC        2
#define DG_SPK_DATA       5

// TTP223
#define DG_TOUCH_TALK     4
#define DG_TOUCH_NEXT     6
#define DG_TOUCH_MODE     7

// BLE / Chronos
#define DG_DEVICE_NAME    "Dirgamochi"
#define DG_CHRONOS_BATTERY 100

// Timing
#define DG_EYE_FRAME_MS   35
#define DG_BUTTON_DEBOUNCE_MS 35
#define DG_LONG_PRESS_MS  900
#define DG_NOTIFICATION_MS 6000
#define DG_NAVIGATION_MS  9000
