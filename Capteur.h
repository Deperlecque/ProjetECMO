#ifndef CAPTEUR_H
#define CAPTEUR_H

#include "HX711.h"
#include "Arduino.h"

class CCapteur {
private:
    HX711 hx711;
    int dtPin;
    int sckPin;
    float calibrationFactor;

public:
    CCapteur(int dtPin, int sckPin);
    ~CCapteur();
    void setupCapteur();
    long getPression();
    void tareSensor();
    void setCalibrationFactor(float factor);
    float getCalibrationFactor();
};

#endif