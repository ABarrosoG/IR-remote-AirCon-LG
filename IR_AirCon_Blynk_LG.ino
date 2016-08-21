#include <IRremoteESP8266.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <BlynkSimpleEsp8266.h>
#include "config.h"

/*char auth[] = "xxxxxxxxxxxxxxxx";
// WLAN data
const char* ssid = "xxxxxxxxxxx";
const char* password = "xxxxxxxxxxxx";*/

bool conectado = false;

// establece pin IR GPIO
IRsend irsend(4);

void setup(){
  Serial.begin(9600);
  Serial.println("pu");
  irsend.begin();
  Blynk.begin(auth, ssid, password); //conexión

  Serial.println("pa");
  delay(100);
  
  // Intentará conectarse 4 veces al servidor blynk
  int con = 0;
   do {
  conectado = Blynk.connect();
  Serial.println("pe");
  con = con+1;
   } while (!conectado && con <=3);
  Serial.println(F("Blynk v" BLYNK_VERSION ": Device started"));
  Serial.println("pi");
}
void loop(){
  if (conectado) {Blynk.run();}

}
BLYNK_WRITE(V0)//OFF AirCon
{
  if (param.asInt()){
    irsend.sendLG(143392849, 28); // codigo decimal de la funcion, nº de bits.
    delay(40);
  }
}
BLYNK_WRITE(V1)//22º cool
{
  if (param.asInt()){
    irsend.sendLG(142608169, 28);
    delay(40);
  }  
}
BLYNK_WRITE(V2)//28º heat
{
  if (param.asInt()){
    irsend.sendLG(142626083, 28);
    delay(40);
  }
}
