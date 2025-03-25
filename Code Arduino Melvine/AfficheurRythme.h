#ifndef AFFICHEURRYTHME_H
#define AFFICHEURRYTHME_H

#include <LiquidCrystal.h>

class CAfficheurRythme {
private:
    LiquidCrystal& lcd;
    float valeurRythme;

public:
    CAfficheurRythme(LiquidCrystal& display); // Constructeur prenant une référence LiquidCrystal
    ~CAfficheurRythme();
    void setupDisplay();
    void lireValeurRythme();
    void afficherRythme(float valeur);
    int calculerMoyenneRythme();
};

#endif