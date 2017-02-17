
/*
    Main
*/
#include <ESP8266WiFi.h>
#include "connectWiFi.h"
#include "manageCamera.h"
#include "schedule.h"

const char* ssid     = "TP-LINK_7B0E";
const char* passwordWifi = "23263345";
const String camera_ip = "192.168.0.70";

const String guardTourPort = "9";
const String recordPort = "1";
const String StationOnePort = "2";
const char* username = "root";
const char* password = "pass";


void loop() {
  // Try to connect the WiFi network as long as the connection is down
  while (!checkConnection()) {
    Serial.println("Wifi connection is down");
    connectWifi(ssid, password);
  }
}


void activateCamera() {
  // sendToCamera(camera_ip, activateVirtualPort (StationOnePort), username, password);
  // delay(10);
  sendToCamera(camera_ip, activateVirtualPort (guardTourPort), username, password);
  delay(100);
  //sendToCamera(camera_ip, activateVirtualPort (guardTourPort), username, password);
  /*
    for (int i = 0; i < 60; i++) {
    delay(1000);
    Serial.print(i);
    }
    Serial.println("");
  */
  sendToCamera(camera_ip, deactivateVirtualPort (guardTourPort), username, password);
  // delay(10);
  // sendToCamera(camera_ip, deactivateVirtualPort (recordPort), username, password);
  // sendToCamera(camera_ip, deactivateVirtualPort (StationOnePort), username, password);
}



/*
   --------------------
     Task: Pirsensor
   --------------------
*/

class PirTask : public Task {
  public:

    const int ledPin1 = 14; // Big lamp
    const int pir = 13; //  Pir sensor

    int pirCounter = 0; // counter to read from pir sensor
    int pirSum = 0; // sum to add up the HIGHs and LOWs from pir sensor

    unsigned long prevTime = 0; // variable that keeps track of a previous time


    void setup() {
      Serial.begin(115200); // behövs inte kanske?
      pinMode(ledPin1, OUTPUT);
      pinMode(pir, INPUT);
    }

    void loop() {
      int pirValue = digitalRead(pir);
      delay(10);
      Serial.println(pirValue);
      delay(100);
      doWithPirValue(pirValue);
      delay(390);
    }

    /*
      uses a reading from pir sensor to calculate a movement
    */
    void doWithPirValue(int pirvalue) {
      if (pirvalue == HIGH) {
        pirCounter = pirCounter + 1;
        pirSum = pirSum + 10;
      }

      if (pirvalue == LOW) {
        pirCounter = pirCounter + 1;
      }
    }

    /*
      checks if there is a movement or not and acts according to that
    */
    void doWhenMove() {
      if (pirCounter == 7) {
        pirCounter = 0;
        if (pirSum == 70) {
          prevTime = millis();
          digitalWrite(ledPin1, HIGH);
        }
        if (pirSum < 70 && ( millis()  - prevTime ) > 10000 ) {
          digitalWrite(ledPin1, LOW);
          prevTime = 0;
        }
        pirSum = 0;
      }
    }

} pirTask;

//----------------------------- END OF PirTask -------------------------------------

/*
   --------------------
     Task: LedTask
   --------------------
*/

class LedTask : public Task {
  public:

    const int ledPin2 = 12; // small lamp
    const int sensorPin = A0; // Analogpin for the pir sensor

    void setup() {
      pinMode(sensorPin, INPUT);
      pinMode(ledPin2, OUTPUT);
    }

    void loop() {
      int sensorValue = analogRead(sensorPin);
      delay(100);
      Serial.println(sensorValue);
      doWithSensorValue(sensorValue);
    }

    /*
      checks if the sensor value goes past the threshold of 75.
    */
    void doWithSensorValue(int sensorvalue) {
      if (sensorvalue > 75) { // LED ON
        digitalWrite(ledPin2, HIGH);
      }
      if (sensorvalue < 75) { // LED OFF
        digitalWrite(ledPin2, LOW);
      }
    }

} ledTask;

//----------------------------- END OF ledTask -------------------------------------

/*
   --------------------
     Task: CameraTask ... TODO
   --------------------
*/

class WifiTask : public Task {
  public:

    void setup() {
      //...
    }

    void loop() {
      while (!checkConnection()) {
        Serial.println("Wifi connection is down");
        connectWifi(ssid, password);
      }
    }
} WifiTask;


void setup() {

  Serial.begin(115200);
  delay(100);

  // Connecting to a WiFi network
  connectWifi(ssid, passwordWifi);
  delay(100);
  activateCamera();
  Scheduler.start(&pirTask);
}


