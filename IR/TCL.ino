#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

const uint16_t kIrLed = 15;
IRsend irsend(kIrLed);

void setup() {
  irsend.begin();
  Serial.begin(115200);
  
}

void loop() {
  delay(10000);
  irsend.sendNikai(0xD5F2A, 24);
  irsend.sendNikai(0xD5F2A, 24);
  irsend.sendNikai(0xD5F2A, 24);
  irsend.sendNikai(0xD5F2A, 24);
  irsend.sendNikai(0xD5F2A, 24);
  irsend.sendNikai(0xD5F2A, 24);
  Serial.println("IR Send");
}
