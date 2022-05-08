#include <ESP8266WiFi.h>
#include "ThingSpeak.h"

#define ADC_VREF_mV    3300.0 // in millivolt
#define ADC_RESOLUTION 4096.0

//-----------Wi-Fi Details---------//
const char* ssid = "GuestC1"; // your wifi SSID name
const char* password = "departamentoc1" ;// wifi pasword
//-------------------------------------------//

//-----------ThingSpeak Details---------//
const char* server = "api.thingspeak.com";
String apiWritekey = "AGBO8BYSVE1F7KH3"; // replace with your THINGSPEAK WRITEAPI key here
//-------------------------------------------//

WiFiClient client;
 
void setup() {
  Serial.begin(115200);
  WiFi.disconnect();
  delay(10);
  WiFi.begin(ssid, password);
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("NodeMcu connected to wifi...");
  Serial.println(ssid);
  Serial.println();
}
 
void loop() {
  int ADC = (analogRead(A0));  /* Read Temperature */
  float milliVolt = ADC * (ADC_VREF_mV / ADC_RESOLUTION);
  float temp = milliVolt / 10;
  
  
  if (client.connect(server,80))
  {  
    String tsData = apiWritekey;
           tsData +="&field1=";
           tsData += String(temp);
           tsData += "\r\n\r\n";
 
     client.print("POST /update HTTP/1.1\n");
     client.print("Host: api.thingspeak.com\n");
     client.print("Connection: close\n");
     client.print("X-THINGSPEAKAPIKEY: "+apiWritekey+"\n");
     client.print("Content-Type: application/x-www-form-urlencoded\n");
     client.print("Content-Length: ");
     client.print(tsData.length());
     client.print("\n\n");  // the 2 carriage returns indicate closing of Header fields & starting of data
     client.print(tsData);
 
     Serial.print("Temperature: ");
     Serial.print(temp);
     Serial.println(" *C");
     Serial.println("\n uploaded to Thingspeak server....");
  }
  client.stop();
 
  Serial.println("Waiting to upload next reading...");
  Serial.println();
  // thingspeak  delay between updates
  delay(3600);
}
