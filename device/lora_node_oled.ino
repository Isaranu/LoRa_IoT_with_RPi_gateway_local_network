#include <SPI.h>
#include <LoRa.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

int counter = 0;
int node_id = 2;
String payload = "";

float t,h;

void shownodedata() {
  
    display.clearDisplay();

    display.setCursor(32,24);
    display.println("Node No. " + String(node_id));

    display.setCursor(32,44);
    display.println("Temp " + String(t));
    display.setCursor(32,54);
    display.println("RH " + String(h));

    display.display();
    delay(1000);
    
}

void showsentdata(){
    display.clearDisplay();

    display.setCursor(32,24);
    display.println("Data");
    display.setCursor(32,44);
    display.println("sending..");

    display.display();
    delay(1000);
}

void setup() {
  
  Serial.begin(115200);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  display.setCursor(36,42);
  display.println("Node test");
  display.setCursor(42,52);
  display.println("No.2");
  display.display();

  Serial.println("LoRa Sender");
  
  LoRa.setPins(D8, D3, D0); // Config SS, RESET, D0 (IRQ)
  
  if (!LoRa.begin(925E6)) { // Config frequency
    Serial.println("Starting LoRa failed!");
    while(1) delay(100);
  }

}

void loop() {

  /* Dummy */
  t = 22.5;
  h = 30.5;

  shownodedata();
  delay(2000);
  
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
  showsentdata();
  LoRa.beginPacket();
  LoRa.print(payload);
  LoRa.endPacket();

  counter++;
  payload = "";

  delay(5000);
}
