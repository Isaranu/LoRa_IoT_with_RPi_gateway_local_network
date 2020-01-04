/*
  Based on ESP8266 Wemos D1 mini
*/
#include <SPI.h>
#include <LoRa.h>
#include <Adafruit_NeoPixel.h>

int counter = 0;
char* child_packet_num;
int child_node_id;
float readTemp, readHumid;

char delimiters[] = ":";
char* val;

#define PIN D2
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);
#define BRIGHT    350               //max led intensity (1-500)
#define INHALE    1250              //Inhalation time in milliseconds.
#define PULSE     INHALE*1000/BRIGHT
#define REST      1000              //Rest Between Inhalations.

void setup() {
  
  Serial.begin(115200);
  
  pixels.begin();
  pixels.setBrightness(64); // Max brightnesss : 255

  /* Initial LoRa */
  //Serial.println("Initial LoRa shield..");
  LoRa.setPins(D8, D3, D0); // Config SS, RESET, D0 (IRQ)
  
  if (!LoRa.begin(925E6)) {
    //Serial.println("Starting LoRa failed!");
    while(1) delay(100);
  }else{
    //Serial.println("LoRa shield ready !");
    }
}

void loop() {
  
  if (LoRa.parsePacket() > 0) {
    
    String receivedPacket = LoRa.readString();
    //Serial.println("-----------------------------");
    //Serial.print("Received payload : ");
    //Serial.print(receivedPacket);
    //Serial.print(" | RSSI is ");
    //Serial.println(LoRa.packetRssi());

    Serial.println(receivedPacket + ":" + LoRa.packetRssi());

    /* Parsing data */
    char buf[256];
    receivedPacket.toCharArray(buf, 256);

    val = strtok(buf, delimiters);
    child_packet_num = val;
    
    val = strtok(NULL, delimiters);
    child_node_id = atoi(val);

    val = strtok(NULL, delimiters);
    readTemp = atof(val);

    val = strtok(NULL, delimiters);
    readHumid = atof(val);

    //Serial.println("child_packet_num : " + String(child_packet_num));
    //Serial.println("child_node_id : " + String(child_node_id));
    //Serial.println("temp : " + String(readTemp));
    //Serial.println("humid : " + String(readHumid));

    LED_received_packet(child_node_id);

  }
  delay(1);
}

void LED_received_packet(int id){

  for(int i=0; i<id; i++){
    pixels.setPixelColor(0, pixels.Color(0,255,0));
    pixels.show();
    delay(50);

    pixels.setPixelColor(0, pixels.Color(0,0,0));
    pixels.show();
    delay(50);
  }
  
}
