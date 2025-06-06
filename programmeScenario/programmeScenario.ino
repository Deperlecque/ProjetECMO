// ==============================================
// Librairies Essentielles pour la Connexion BDD
// ==============================================
#include <WiFi.h>        // Pour la connectivité WiFi 
#include <MySQL_Connection.h> // Pour la connexion MySQL
#include <IPAddress.h>        // Pour gérer les adresses IP

// ==============================================
// Headers Personnalisés
// ==============================================
#include "CBDD.h" // Doit contenir la définition de votre classe CBDD
                 // avec les méthodes connect() et sendData(...)

// ==============================================
// Configuration Réseau Wi-Fi
// ==============================================
const char* ssid = "ECMOPi";
const char* password = "smoothie";

// ==============================================
// Configuration MySQL (BDD)
// ==============================================
IPAddress mysql_server_ip_obj;
char mysql_server_str[] = "10.42.0.1";
int mysql_port = 3306;
char user_mysql[] = "ECMOPi";
char password_db_mysql[] = "smoothie";
char database_name_mysql[] = "mydb";
char table_name_mysql[] = "Donnees_exercice";

// ==============================================
// Objets Globaux pour la Connexion BDD
// ==============================================
WiFiClient wifiClientForBDD;
MySQL_Connection mysqlCon(&wifiClientForBDD);

CBDD bddManager(mysqlCon, mysql_server_ip_obj, mysql_port, user_mysql, password_db_mysql, database_name_mysql, table_name_mysql);

// Variables pour simuler un envoi de données périodique
unsigned long previousMillisSendData = 0;
const long intervalSendData = 10000; // Envoyer des données toutes les 10 secondes par exemple

// ==============================================
// Fonction d'Initialisation (Setup)
// ==============================================
void setup() {
    Serial.begin(9600);
    while (!Serial);

    Serial.println("Systeme: Demarrage...");

    if (!mysql_server_ip_obj.fromString(mysql_server_str)) {
        Serial.print("Systeme: Erreur - Adresse IP MySQL invalide: ");
        Serial.println(mysql_server_str);
        while (1);
    }
    Serial.print("Systeme: IP du serveur MySQL configuree: ");
    Serial.println(mysql_server_ip_obj);

    Serial.print("Systeme: Connexion au Wi-Fi '");
    Serial.print(ssid);
    Serial.println("'...");
    WiFi.begin(ssid, password);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nSysteme: Connexion Wi-Fi etablie !");
        Serial.print("Systeme: Adresse IP locale: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nSysteme: Echec de la connexion Wi-Fi.");
        return;
    }

    Serial.println("Systeme: Initialisation terminee. Pret a envoyer des donnees.");
}

// ==============================================
// Boucle Principale (Loop) - Gestion de la connexion et envoi de données
// ==============================================
void loop() {
    // 1. Gestion de la connexion WiFi
    if (WiFi.status() != WL_CONNECTED) {
        static unsigned long lastWifiReconnectAttempt = 0;
        unsigned long currentMillis = millis();
        if (currentMillis - lastWifiReconnectAttempt > 15000) {
            Serial.println("Systeme: WiFi deconnecte. Tentative de reconnexion...");
            bddManager.setConnected(false); // Informer CBDD que la connexion est perdue
            WiFi.begin(ssid, password);
            lastWifiReconnectAttempt = currentMillis;
        }
        return; // Attendre la reconnexion WiFi avant de tenter d'envoyer des données
    }

    // 2. Envoi de données à intervalle régulier
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillisSendData >= intervalSendData) {
        previousMillisSendData = currentMillis;

        // Simuler l'acquisition de données vitales
        // Dans votre code réel, ces valeurs viendraient de capteurs ou d'autres calculs.
        int valeurSAT = random(90, 100); // Saturation entre 90 et 99%
        int valeurCapnie = random(35, 45);   // Capnographie (EtCO2) en mmHg
        int valeurECG = random(60, 100);     // Rythme cardiaque (BPM)
        int valeurPressionArterielleSys = random(110, 130); // Pression systolique (simplifié à une valeur)
        // La fonction sendData attend une seule valeur pour pressionArterielle.
        // Si vous avez systolique/diastolique, vous devrez adapter la méthode sendData ou stocker différemment.
        int valeurFrequenceRespiratoire = random(12, 20); // Fréquence respiratoire par minute

        Serial.println("\nSysteme: Preparation de l'envoi des donnees vitales...");
        Serial.print("  SAT: "); Serial.println(valeurSAT);
        Serial.print("  Capnie: "); Serial.println(valeurCapnie);
        Serial.print("  ECG: "); Serial.println(valeurECG);
        Serial.print("  PressionArterielle: "); Serial.println(valeurPressionArterielleSys);
        Serial.print("  FrequenceRespiratoire: "); Serial.println(valeurFrequenceRespiratoire);

        // Appel de la méthode sendData de l'objet bddManager
        // La méthode sendData dans CBDD devrait gérer la connexion si elle est perdue.
        if (bddManager.sendData(valeurSAT, valeurCapnie, valeurECG, valeurPressionArterielleSys, valeurFrequenceRespiratoire)) {
            Serial.println("Systeme: Donnees envoyees a la BDD avec succes.");
        } else {
            Serial.println("Systeme: Echec de l'envoi des donnees a la BDD.");
            // La méthode sendData (et connect) de CBDD gère déjà l'état _isConnected
        }
    }
    delay(10000); // Un petit délai pour ne pas surcharger la boucle si rien d'autre n'est fait.
}
