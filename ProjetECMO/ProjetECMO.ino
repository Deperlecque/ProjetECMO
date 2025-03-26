// Déclaration de la broche du potentiomètre
#define potPin A0

// Décarations des broches du pont en 
#define moteurPin1 A1 // PWM
#define moteurPin2 A2 // PWM

// Déclaration des variables
int potValue = 0;
int consigne = 0;
int erreur = 0;
int engineSpeed = 0;

void setup() {
  pinMode(moteurPin1, OUTPUT);
  pinMode(moteurPin2, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Lecture de la position du potentiomètre
  potValue = analogRead(potPin);

  // Définir la consigne (par exemple, la moitié de la course)
  consigne = 512;

  // Calcul de l'erreur
  erreur = consigne - potValue;

  // Contrôle du moteur en fonction de l'erreur
  engineSpeed = map(abs(erreur), 0, 1023, 0, 255); // Ajuster la vitesse

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

  // Affichage des valeurs pour le débogage
  Serial.print("PotValue: ");
  Serial.print(potValue);
  Serial.print(", Erreur: ");
  Serial.print(erreur);
  Serial.print(", Vitesse: ");
  Serial.println(engineSpeed);

  delay(10); // Ajuster le délai selon les besoins
}
