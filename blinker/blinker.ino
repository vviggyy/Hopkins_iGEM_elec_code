#include <Wire.h>
#include <PWM.h>

#define CLK 16000000UL
#define shift 90
#define PRESCALER 1

#define address 0x36

int LED_PIN = 3;

void setup() {
  
  Wire.begin(address);
  pinMode(LED_PIN, OUTPUT);
  
}

void parseMsg(char msg[]) {
  
  boolean ledOn = msg[0];

  if (ledOn) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
  
  
}

void loop() {

  Wire.onReceive(parseMsg);
  

}
