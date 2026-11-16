
#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
    Serial.begin(115200);

    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);

    // Draw a large green right arrow
    int cx = 160;
    int cy = 120;

    // Arrow shaft
    tft.fillRect(cx - 90, cy - 20, 120, 40, TFT_GREEN);

    // Arrow head
    tft.fillTriangle(
        cx + 70, cy,
        cx + 10, cy - 60,
        cx + 10, cy + 60,
        TFT_GREEN
    );

    // Label
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(90, 200);
    tft.println("GO RIGHT");
}

void loop() {
}
