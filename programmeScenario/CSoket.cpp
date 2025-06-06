/*#include "CSocket.h"
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
CSocket::CSocket()
{

}

CSocket::~CSocket()
{
  
}

void CSocket::ConnexionWifi()
{
  const char* ssid = "ECMOPi";
  const char* password =  "smoothie";

  Serial.println("\n");

  // Connection au Wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Connection au Wifi..");
  }
    
  IPAddress local_IP(172,29,19,214);   
  IPAddress gateway(172, 29, 0, 1);
  IPAddress subnet(255, 255, 128, 0); 
  IPAddress primaryDNS(172, 16, 0, 100);  
  IPAddress secondaryDNS(8,8,8,8); 

  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) 
  {   
    Serial.println("STA Failed to configure");
  }

  Serial.println("\n");  // Affichage "Connexion établie" et de l'adresse IP
  Serial.println("Connexion etablie !");
  Serial.print("Adresse IP: ");
  Serial.println(WiFi.localIP());
}

void CSocket::Socket(float frequenceRespiratoire, float SAT, float pressionArterielle, float capnie, float electrocardiogramme)
{
  wifiServer.begin();
  WiFiClient client = wifiServer.available();
  Serial.print("client ");
  Serial.println(client);
  if (client) 
  {
    Serial.print("client  disponible ");
    Serial.println(client);
    while (client.connected()) 
    {
      while (client.available()>0) 
      {
        char c = client.read();
        Serial.write(c);
    
        if (c == '?' )
        {  
          client.print( "%"+String(frequenceRespiratoire)+";"+String(SAT)+";"+String(pressionArterielle)+";"+String(capnie)+";"+String(electrocardiogramme)+"!");   
        }
       delay(100);  
      } 
    }
    client.stop();
    Serial.println("Client disconnected");  
  }
}*/