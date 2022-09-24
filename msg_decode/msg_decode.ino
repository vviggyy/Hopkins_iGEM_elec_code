#include <Wire.h>

#define SLAVE_ADDRESS 0x36
//#define MEM_ADDRESS 0x3F00 //SRAM

int LED_PIN = 13;
int numTransactions = 1;
boolean led_status = false;

int dig1 = 0;
int dig2 = 0;
int dig3 = 0;

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
  
  int x = Wire.read();
  
  Serial.println("RECIEVED INT:");
  Serial.println(x);
  Serial.println();

  char buf[100];
  sprintf(buf, "%03i", x); //separates integers into chars

  dig1 = buf[0];
  
  dig2 = buf[1];
  dig3 = buf[2];

  Serial.println("Digit 1:");
  Serial.println(dig1);
  Serial.println();

  Serial.println("Digit 2:");
  Serial.println(dig2);
  Serial.println();

  Serial.println("Digit 3:");
  Serial.println(dig3);
  Serial.println();

  if (dig1 != 0 && dig2 != 0 && dig3 != 0) {
    digitalWrite(LED_PIN, !led_status);
  }
  
  
  
//  //if(x = B00000001) {
//      if (numTransactions % 2 == 0) {
//        digitalWrite(LED_PIN, HIGH);
//      } else {
//        digitalWrite(LED_PIN, LOW);
//      }
//      
//      numTransactions = numTransactions + 1;
//  //}
}
