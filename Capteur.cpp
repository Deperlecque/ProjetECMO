#include "Capteur.h"
#include "Arduino.h"

CCapteur::CCapteur(int dtPin, int sckPin) : dtPin(dtPin), sckPin(sckPin) {
  hx711.begin(dtPin, sckPin);
  calibrationFactor = 443.0;
}

void CCapteur::setupCapteur() {
  hx711.set_scale(calibrationFactor);
  hx711.tare();
}

long CCapteur::getPression() { 
  if (hx711.is_ready()) {
    return hx711.get_units();
  } else {
    Serial.println("HX711 non prêt");
    return 0; // Ou une autre valeur d'erreur
  }
}

void CCapteur::tareSensor() {
  hx711.tare();
}

void CCapteur::setCalibrationFactor(float factor) {
  calibrationFactor = factor;
  hx711.set_scale(calibrationFactor);
}

float CCapteur::getCalibrationFactor() {
  return calibrationFactor;
}