#include <WiFi.h>      // <-- Bibliothèque WiFi pour l'Arduino UNO R4 WiFi

// --- Configuration Réseau Wi-Fi ---
const char* ssid = "Galaxy_S22734C";     // <<--- Votre nom de réseau
const char* password = "thomas2004"; // <<--- Votre mot de passe réseau

// --- Configuration IP Statique (Ces variables ne sont plus utilisees pour la configuration) ---
// Elles sont commentees ou retirees car on utilise DHCP.
// IPAddress static_ip(172, 29, 21, 20);
// IPAddress gateway(10, 42, 0, 1);
// IPAddress subnet(255, 255, 128, 0);
// IPAddress dns1(172, 16, 0, 100);
// IPAddress dns2(8, 8, 8, 8);


// --- Configuration du Serveur ---
WiFiServer server(80);

// --- Contenu de la page HTML (inchangé) ---
const char* htmlContent = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Page Web Arduino UNO R4 WiFi (DHCP)</title>
  <meta charset="UTF-8">
</head>
<body>
  <h1>Bonjour depuis mon Arduino UNO R4 WiFi (gestion DHCP) !</h1>
  <p>Ceci est une page web servie par l'Arduino en utilisant WiFiServer et WiFiClient directement.</p>
  <p>Adresse IP : <span id="ip-address">Chargement...</span></p>

  <script>
    // Ce script s'execute dans le navigateur, pas sur l'Arduino.
    // Il affiche l'adresse IP a partir de l'URL a laquelle vous avez accede.
    document.getElementById('ip-address').innerText = window.location.host;
  </script>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Demarrage du serveur web Arduino UNO R4 WiFi (DHCP)...");

  // --- Connexion WIFI en utilisant DHCP ---

  // Pas d'appel a WiFi.config() ici, donc la configuration se fait via DHCP

  Serial.print("Connexion au reseau Wi-Fi : ");
  Serial.print(ssid);
  Serial.println("...");

  // Commence la connexion WiFi (obtient l'IP via DHCP)
  WiFi.begin(ssid, password);

  // Attendre que la connexion Wi-Fi soit établie
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connexion Wi-Fi etablie avec succes !");

  // Afficher l'adresse IP attribuee par le serveur DHCP
  IPAddress currentIP = WiFi.localIP(); // Récupère l'IP attribuee par DHCP
  Serial.print("Le serveur web est accessible a l'adresse IP : ");
  Serial.println(currentIP);
  Serial.println("Ouvrez un navigateur et allez a cette adresse.");

  // Démarrer le serveur qui écoute les connexions entrantes
  server.begin();
  Serial.println("Serveur HTTP demarre (manuel).");
}

void loop() {
  // Écoute les connexions entrantes des clients web
  WiFiClient client = server.available();

  if (client) { // Si un client se connecte
    Serial.println("Nouveau client connecté.");

    boolean currentLineIsBlank = true;
    char clientChar;

    while (client.connected()) {
      if (client.available()) {
        clientChar = client.read();

        if (clientChar == '\n' && currentLineIsBlank) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          client.println(htmlContent);
          break;
        }

        if (clientChar == '\n') {
          currentLineIsBlank = true;
        } else if (clientChar != '\r') {
          currentLineIsBlank = false;
        }
      }
    }

    delay(1);
    client.stop();
    Serial.println("Client deconnecte.");
  }
}