#include "ESP8266WiFi.h"
#include <SPI.h>
#include <LoRa.h>
#include "DHT.h"
#include <Adafruit_NeoPixel.h>

int counter = 0;
int node_id = 1;
String payload = "";

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

float t,h;

#define PIN D2
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);
#define BRIGHT    350               //max led intensity (1-500)
#define INHALE    1250              //Inhalation time in milliseconds.
#define PULSE     INHALE*1000/BRIGHT
#define REST      1000              //Rest Between Inhalations.

void setup() {
  
  Serial.begin(115200);
  dht.begin();
  pixels.begin();
  pixels.setBrightness(64); // Max brightnesss : 255
  Serial.println("LoRa Sender");
  
  LoRa.setPins(D8, D3, D0); // Config SS, RESET, D0 (IRQ)
  
  if (!LoRa.begin(925E6)) { // Config frequency
    Serial.println("Starting LoRa failed!");
    while(1) delay(100);
  }

}

void loop() {

  LED_breath();  

  /* - DHT sensor reading - */
  t = dht.readTemperature();
  h = dht.readHumidity();

  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  /* Set payload */
  //payload = String(counter, HEX);
  payload = String(counter);
  payload += ":";
  payload += String(node_id, HEX);
  payload += ":";
  payload += String(t, 1);
  payload += ":";
  payload += String(h, 1);

  Serial.println(payload);

  /* send packet */
  LoRa.beginPacket();
  LoRa.print(payload);
  LoRa.endPacket();
  LED_shooting();

  counter++;
  payload = "";

  delay(5000);
}

void LED_breath(){

  for (int i=1;i<BRIGHT;i++){
    
    pixels.setPixelColor(0, pixels.Color(255,255,255));
    pixels.show();
    
    delayMicroseconds(i*10);         
    
    pixels.setPixelColor(0, pixels.Color(0,0,0));
    pixels.show();
    
    delayMicroseconds(PULSE-i*10);  
    delay(0);                      
  }

  for (int i=BRIGHT-1;i>0;i--){

    pixels.setPixelColor(0, pixels.Color(255,255,255));
    pixels.show();
    
    delayMicroseconds(i*10);         
     
    pixels.setPixelColor(0, pixels.Color(0,0,0));
    pixels.show();
    
    delayMicroseconds(PULSE-i*10);
    i--;
    delay(0);                      
  }
  delay(REST);
}

void LED_shooting(){

  for(int i=0; i<3; i++){
      pixels.setPixelColor(0, pixels.Color(255,0,0));
      pixels.show();
      delay(25);

      pixels.setPixelColor(0, pixels.Color(0,0,0));
      pixels.show();
      delay(25);
    }

}
