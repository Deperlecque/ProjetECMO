#include "CPotentiometre.h"
#include <Arduino.h>
#define moteurPin1 9
#define moteurPin2 10
#define potPin A0

CPotentiometre::CPotentiometre()
{

}

CPotentiometre::~CPotentiometre()
{

}

void CPotentiometre::deplacerPotentiometre(int positionCible)
{
  int potValue = 0;
  int consigne = 512; // Devrait probablement être 'positionCible' ou basé sur 'positionCible'
  int erreur = 0;
  int engineSpeed = 0;

  // Lecture de la position du potentiomètre
  potValue = analogRead(potPin);

  // Calcul de l'erreur
  erreur = consigne - potValue;
  engineSpeed = 50*map(abs(erreur), 0, 1023, 0, 255); // Map sur la plage analogWrite

  if (erreur > 0) {
    // Déplacer le moteur dans une direction
    analogWrite(moteurPin1, engineSpeed);
    analogWrite(moteurPin2, 0);
  } else if (erreur < 0) {
    // Déplacer le moteur dans l'autre direction
    analogWrite(moteurPin1, 0);
    analogWrite(moteurPin2, engineSpeed);
  } else {
    // Arrêter le moteur
    analogWrite(moteurPin1, 0);
    analogWrite(moteurPin2, 0);
  }
}