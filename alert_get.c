/*
  Rui Santos
  Complete project details at Complete project details at https://RandomNerdTutorials.com/esp8266-nodemcu-http-get-post-arduino/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const int ledr= 12; //pin = D6

const char* ssid = "Extra";
const char* password = "extra20000910";
const char fingerprint[] PROGMEM = "51f067026201f4fb917533b27febcc5cbbe0550d";
const int httpsPort = 443;

void setup(){
  Serial.begin(115200);

  pinMode(ledr, OUTPUT);

  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print("*");
  }
}

void loop(){

  if (WiFi.status() == WL_CONNECTED) {

    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setFingerprint(fingerprint);
    HTTPClient https;

    https.begin(*client, "https://sui-prevention.bdrip.org/ok.json");
    int httpCode = https.GET();

    if (httpCode > 0){
      const char* json = https.getString().c_str();
      Serial.println(json);

      const size_t capacity = JSON_OBJECT_SIZE(3) + 30;
      DynamicJsonDocument doc(capacity);

      DeserializationError error = deserializeJson(doc, json);

      if (error){
        Serial.println(F("desrializeJson() Failled："));
        Serial.println(error.f_str());
        return;
      }
      
      int red = doc["red"];

      if (red != digitalRead(ledr)){
        digitalWrite(ledr, red);
      }

      Serial.println(F("Json："));
      Serial.println(red);     
    }
    https.end(); 
    
  }
  delay(500);
}


  


  
