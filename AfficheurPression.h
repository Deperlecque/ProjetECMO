#ifndef AFFICHEUR_H
#define AFFICHEUR_H

#include <U8g2lib.h>

class CAfficheurPression {
private:
    U8G2_SSD1306_128X64_NONAME_F_HW_I2C& oled;

public:
    CAfficheurPression(U8G2_SSD1306_128X64_NONAME_F_HW_I2C& display);
    ~CAfficheurPression();
    void afficherPression(long valeurPression, const char* unite = " Kg");
    void setupDisplay();
};

#endif