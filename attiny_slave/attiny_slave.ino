#include <Wire.h>

//pins for PWM, from circuit diagram
int pwm1 = 3;
int pwm2 = 5;


//incoming msg from master contains 4 bits
//long msg = null;

boolean enable = null; // 1 bit
int freqOpt = null; // 2 bits {0: 0 kHz, 1: 167 kHz, 2: 333 kHz, 500 kHz} 
boolean wave = null; // 1 bit {true: step wave, false: DC performance wave}

int frequency = null;


void setup() {
  Wire.begin(address);
  Wire.onReceive(parseMsg);
  
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
}

void parseMsg(char msg[]){
  
  enable = msg[0]
  freqOpt = strtol(msg[1:2], NULL, 2);
  wave = msg[3]

  switch(freqOpt){
    case 0:
      frequency = 0;
      break;
    case 1:
      frequency = 167;
      break;
    case 2: 
      frequency = 333;
      break;
    case 3:
      frequency = 500;
      break;
  }
 
}
void loop() {
  delay(100);
  if (enable == '1') {
    //send PWM1
    //send PWM2
   
  } else {
    continue; //move back
  }
}
