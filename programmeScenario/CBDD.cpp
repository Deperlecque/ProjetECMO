#include "CBDD.h" // Include the renamed header
#include <MySQL_Cursor.h>
#include <stdio.h> // Pour sprintf

// Selon la plateforme, des includes supplémentaires peuvent être nécessaires
// Par exemple, pour Arduino (Serial, millis) :
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
// Pour IPAddress (par exemple avec ESP32) :
// #include <WiFi.h> // Ou Ethernet.h, etc.

// Constructor updated to CBDD
CBDD::CBDD(MySQL_Connection& con, IPAddress& serverIP, int port, const char* user, const char* pass, const char* db, const char* table)
    : _mysqlCon(con), _serverIP(serverIP), _port(port), _user(user), _password(pass), _dbName(db), _tableName(table),
      _isConnected(false), _lastErrorTime(0), _reconnectInterval(30000)
{
}

// Destructor updated to CBDD
CBDD::~CBDD() {
    if (_isConnected) {
        _mysqlCon.close();
    }
}

// Methods now belong to CBDD
bool CBDD::isConnected() {
    return _isConnected;
}

void CBDD::setConnected(bool status) {
    _isConnected = status;
    if (!status) {
        _lastErrorTime = millis();
    }
}

bool CBDD::connect() {
    if (_isConnected) {
        return true;
    }

    Serial.print("CBDD: Tentative de connexion au serveur MySQL ");
    Serial.print(_serverIP); // Note: L'affichage direct de IPAddress peut varier selon la plateforme.
                             // _serverIP.toString() peut être utilisé sur ESP32 par exemple.
    Serial.println("...");

    if (_mysqlCon.connect(_serverIP, _port, (char*)_user, (char*)_password, (char*)_dbName)) {
        Serial.println("CBDD: Connected to MySQL successfully!");
        _isConnected = true;
        return true;
    } else {
        Serial.println("CBDD: MySQL connection FAILED.");
        _isConnected = false;
        _lastErrorTime = millis();
        return false;
    }
}

// Méthode sendData corrigée et mise à jour
bool CBDD::sendData(int SAT, int Capnie, int ECG, int pressionArterielle, int frequenceRespiratoire) {
    if (!_isConnected) {
        Serial.println("CBDD: Not connected to MySQL. Attempting to reconnect...");
        if (!connect()) {
            Serial.println("CBDD: Reconnection failed. Skipping data send.");
            return false;
        }
    }

    // Les anciennes variables 'Pression', 'pressionStr', 'Rythme', 'dureeExercice', 'dureeSQL'
    // ont été supprimées car elles sont remplacées ou incompatibles avec les nouveaux paramètres
    // et la structure de la requête SQL.

    char sqlBuffer[300]; // Taille du buffer ajustée par sécurité
    
    // La requête SQL utilise maintenant les 5 paramètres entiers passés à la fonction.
    // Elle suppose que la table _tableName possède des colonnes nommées
    // SAT, Capnie, ECG, pressionArterielle, et frequenceRespiratoire.
    sprintf(sqlBuffer, "INSERT INTO %s (SAT, Capnie, ECG, pressionArterielle, frequenceRespiratoire) VALUES (%d, %d, %d, %d, %d)",
            _tableName, SAT, Capnie, ECG, pressionArterielle, frequenceRespiratoire);

    Serial.print("CBDD: Executing SQL: ");
    Serial.println(sqlBuffer);

    MySQL_Cursor cursor(&_mysqlCon);

    if (cursor.execute(sqlBuffer)) {
        Serial.println("CBDD: Data INSERTED successfully!");
        if (cursor.get_rows_affected() > 0) {
            Serial.print(cursor.get_rows_affected());
            Serial.println(" row(s) affected.");
            return true;
        } else {
            Serial.println("CBDD: Data insertion reported 0 rows affected.");
            // Pour un INSERT, 0 ligne affectée est généralement inattendu.
            // Cela peut indiquer un problème (ex: contrainte de clé dupliquée avec IGNORE).
            // On considère cela comme un échec partiel de l'objectif d'insertion.
            return false;
        }
    } else {
        Serial.println("CBDD: FAILED to insert data.");
        _isConnected = false; // Mettre à jour le statut de connexion en cas d'échec de la requête
        _lastErrorTime = millis();
        return false;
    }
}