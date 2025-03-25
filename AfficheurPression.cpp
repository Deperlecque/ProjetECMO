#include <Wire.h>
#include "AfficheurPression.h"
#include "Arduino.h"
#include <U8g2lib.h>

CAfficheurPression::CAfficheurPression(U8G2_SSD1306_128X64_NONAME_F_HW_I2C& display) : oled(display) {}

CAfficheurPression::~CAfficheurPression() {}

void CAfficheurPression::setupDisplay() {
    oled.begin();
}

void CAfficheurPression::afficherPression(long valeurPression, const char* unite) {
    oled.clearBuffer();
    oled.setFont(u8g2_font_profont10_tf);
    oled.drawStr(0, 10, "Pression:");
    oled.setCursor(0, 25);
    oled.print(valeurPression);
    oled.print(unite);
    oled.sendBuffer();
}