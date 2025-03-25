#include "AfficheurRythme.h"
#include <LiquidCrystal.h>

CAfficheurRythme::CAfficheurRythme(LiquidCrystal &display) : lcd(display) {}

CAfficheurRythme::~CAfficheurRythme() {}

void CAfficheurRythme::setupDisplay() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Rythme:");
}

void CAfficheurRythme::lireValeurRythme() {
  // Code pour lire la valeur du rythme à partir du capteur
  // Exemple:
  valeurRythme = 65.0; // Remplacer par la lecture réelle du capteur
}

void CAfficheurRythme::afficherRythme(float valeur) {
  lcd.setCursor(0, 1);
  lcd.print(valeur);
  lcd.print(" bpm"); // Ajout de l'unité "bpm"
}

int CAfficheurRythme::calculerMoyenneRythme() {
  // Code pour calculer la moyenne du rythme sur une période
  // Exemple:
  return static_cast<int>(valeurRythme); // Exemple de retour (à remplacer par le calcul réel)
}