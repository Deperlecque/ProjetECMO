#include "Systeme.h"
#include "Requete.h"
#include "Capteur.h"
#include "AfficheurPression.h"
#include "AfficheurRythme.h"

#include <Wire.h>
#include <Ethernet.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <LiquidCrystal.h>
#include "HX711.h"
#include "Arduino.h"
#include "rgb_lcd.h"

#define DT_PIN 2
#define SCK_PIN 3
#define AFFRYTHME 4

CCapteur capteur(DT_PIN, SCK_PIN); // Correction : Capteur au lieu de WeightSensor
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
CAfficheurPression afficheurPression(u8g2);
LiquidCrystal lcd(20, 19, 18, 17, 16, 15);
CAfficheurRythme afficheurRythme(lcd);

byte mac= {0xCA, 0xCA, 0xE7, 0x94, 0x00, 0x73}; // Correction : Déclaration du tableau mac
IPAddress ip(172, 29, 21, 100);
EthernetServer server(80);

int obtenirRythme() {
    // Insérez ici le code pour lire votre capteur de rythme cardiaque
    // et retourner la valeur du rythme.
    return 70; // Exemple : retourne une valeur de rythme fixe pour le test
}

void setup() {
    Serial.begin(9600);
    capteur.setupCapteur();
    afficheurPression.setupDisplay();

    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Projet ECMO");
    lcd.setCursor(1, 1);
    lcd.print("Alex, Thomas, Melvine");
    delay(2000);

    Ethernet.begin(mac, ip);
    server.begin();
    Serial.print("Serveur web actif sur : ");
    Serial.println(Ethernet.localIP());

    afficheurRythme.setupDisplay();
}

void loop() {
    long pression = capteur.getPression();
    afficheurPression.afficherPression(pression);

    int rythme = obtenirRythme();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Rythme: ");
    lcd.print(rythme);
    lcd.print(" bpm");
    lcd.setCursor(0, 1);
    delay(100);
}