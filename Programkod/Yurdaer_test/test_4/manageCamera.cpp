
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
/**
   This function will send a request to the server
*/
String sendToCamera ( String command, String host ) {
  HTTPClient httpClient;
  String http_response;
  Serial.print("connecting to ");
  Serial.println(host);
  httpClient.begin("http://" + host + command); //HTTP
  Serial.println("[HTTP] GET...\n");
  int httpCode =  httpClient.GET();


  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    // file found at server
   // if (httpCode == HTTP_CODE_OK) {
      http_response =  httpClient.getString();

  //  }
  }
  
  //If connection not successful
  else {

    http_response = "connection failed";
  }
  httpClient.end();
  return http_response;

}

String getTime() {
  WiFiClient client;
  while (!!!client.connect("google.com", 80)) {
    Serial.println("connection failed, retrying...");
  }

  client.print("HEAD / HTTP/1.1\r\n\r\n");
  while(!!!client.available()) {
     yield();
  }

  while(client.available()){
    if (client.read() == '\n') {    
      if (client.read() == 'D') {    
        if (client.read() == 'a') {    
          if (client.read() == 't') {    
            if (client.read() == 'e') {    
              if (client.read() == ':') {    
                client.read();
                String theDate = client.readStringUntil('\r');
                client.stop();
                return theDate;
              }
            }
          }
        }
      }
    }
  }
}

/**
   Controlling the camera movement by passing in int arguments for the desired angle
   as int and the camera number as int.
*/
String moveCameraHorizontal (int angle, int camera_no) {
  String url = "/axis-cgi/com/ptz.cgi?rpan=" + String(angle) + "&camera=" + String(camera_no);
  return url;
}


String moveCameraVertical (int angle, int camera_no) {
  String url = "/axis-cgi/com/ptz.cgi?rtilt=" + String(angle) + "&camera=" + String(camera_no);
  return url;
}
String autoFocus (String OnOff, String camera_no) {
  String url = "/axis-cgi/com/ptz.cgi?autofocus=" + OnOff + "&camera=" + String(camera_no);
  return url;
}
String autoIris (String OnOff, String camera_no) {
  String url = "/axis-cgi/com/ptz.cgi?autoiris=" + OnOff + "&camera=" + String(camera_no);
  return url;
}
/*Continuous pan/tilt motion.
  Positive values mean right (pan) and up (tilt),
  negative values mean left (pan) and down (tilt). "0,0"
  means stop.
*/
String continuousPanTiltMove (int panSpeed, int tiltSpeed, int camera_no){
   String url = "/axis-cgi/com/ptz.cgi?continuouspantiltmove=" +String(panSpeed)+"," +String(tiltSpeed) + "&camera=" + String(camera_no);
  return url;
}

String GetTimeCamera () {
  String url = "/axis-cgi/date.cgi?action=get";
  return url;
}



