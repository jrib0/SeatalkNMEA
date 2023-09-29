/*
    for the serial to wifi v1.0 dm-101 module
    configured as 4800 to NMEA0183 (raspberry pi kplex server)
    this is the version currently uploaded
*/

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#ifndef STASSID
#define STASSID "NMEA0183"
//#define STASSID "Chimera D"
#define STAPSK  "jamesribrown8"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

//const char* host = "192.168.1.119";
const char* host = "192.168.3.1";
const uint16_t port = 10110;
//const int adc = A0;

ESP8266WiFiMulti WiFiMulti;

WiFiClient client;

void setup() {
  Serial.begin(4800);
  Serial.println("");
  Serial.println("(c) James Brown.  NMEA-WIFI for STALK-ST bridge 4800");

  // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password); //using multi means it will automatically try to reconnect
  
  Serial.print("Waiting for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);

  Serial.print("Connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);

  // Use WiFiClient class to create TCP connections
  if (!client.connect(host, port)) {
    Serial.println("Connection to NMEA Server failed");
    return;
  }
  else Serial.println("Connected OK to NMEA Server ");
}


void loop() {
  int value=0;
  String line;
  while (client.connected())  //got to be connected for this to work
  { //read any available wifi and send to serial
    if (client.available()) //this works fine
    {
      line = client.readStringUntil('\n');
      Serial.println(line);      
    }

    //read from the serial line and sends to the wifi
    if (Serial.available())
    {
      line = Serial.readStringUntil('\r');      
      client.println(line);      
      Serial.flush();
    }
    //value=analogRead(adc);
    //Serial.println(value);
  }
  if (!client.connected()) {
    if (!client.connect(host, port)) {
    Serial.println("Reconnection to NMEA Server failed");
    delay(2000);
  } else Serial.println("Reconnected OK to NMEA Server ");}
}

// to burn this to the serial to wifi module generic 8266, flash size 1MB 64KB, 26Mhz 115200
