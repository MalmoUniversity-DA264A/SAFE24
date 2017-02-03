

/*
    Simple HTTP get webclient test
*/
#include <ESP8266WiFi.h>
#include "connectWiFi.h"
#include "manageCamera.h"
const char* ssid     = "TP-LINK_7B0E";
const char* password = "23263345";
const char* camera_ip = "192.168.0.70";
const int httpPort = 80;

//The esp module IP, gateway and subnet adresses
IPAddress esp_ip (192, 168, 0, 21);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  // We start by connecting to a WiFi network
  connectWifi(esp_ip, gateway, subnet, ssid, password);
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(2000);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
    Serial.println("1");
sendToCamera(client,moveCamera(150, 1),camera_ip,httpPort );
  Serial.println("22222222222222222222222222222222222222");
  

}


