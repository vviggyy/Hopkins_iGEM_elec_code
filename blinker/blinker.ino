#include <Wire.h>

#define SLAVE_ADDRESS 0x36
#define MEM_ADDRESS 0x3F00 //SRAM

int LED_PIN = 4;

void setup() {
  
  Wire.begin(SLAVE_ADDRESS);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);

  Wire.onReceive(parseMsg);
}

void loop() {
 
  delay(100);
}


void parseMsg(int eventNum) {
  
  while (1 < Wire.available()) { //if there is still more than 1 byte in the buffer

    char c = Wire.read();
    if (c == '00000001') {
      digitalWrite(LED_PIN, HIGH);
      delay(1000);
      digitalWrite(LED_PIN, LOW);
    }
  }
  
  int x = Wire.read();
  if(x = B00000001) {
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
  }
 
  
}
