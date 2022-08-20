#include <Wire.h>
#include <PWM.h>

#define CLK 16000000UL // 20 mHz or 16 mHz, figure out CLK 
#define shift 90

#define address 0x36 //define slave address, Q: need to <<1 ?  

//pins for PWM, from circuit diagram
int pwm1 = 3;
int pwm2 = 5;

int pwm1_toggle = null;
int pwm2_toggle = null;

//incoming msg from master contains 4 bits
//long msg = null;

boolean enable = null; // 1 bit
int freqOpt = null; // 2 bits {0: 0 kHz, 1: 167 kHz, 2: 333 kHz, 500 kHz} 
boolean wave = null; // 1 bit {true: step wave, false: DC performance wave}

int frequency = null;


void setup() {
  
  Wire.begin(address);
  
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);

  InitTimerSafe(); //from PWM library, initializes timers to 0
}

void parseMsg(char msg[]){
  
  enable = msg[0]
  freqOpt = strtol(msg[1:2], NULL, 2); //receive a standard i2c message as length of 8 bytes, parse that not a string 
  //check if first four bits and last four bits are equal and if message is garbled then ignore
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

 SetPinFrequencySafe(pwm1, frequency) //set freq pwm1
 SetPinFrequencySafe(pwm2, frequency) //set freq pwm2
}
void loop() {
  
  Wire.onReceive(parseMsg); //type check; receive as string or bits?

  delay(100);
  if (enable == '1') {

    //pwmWrite(pwm1, 127) //50% duty cycle pwm1
    //pwmWrite(pwm2, 127) //50% duty cycle pwm2
    //TODO phase shift

    pwm1_toggle = (CLK / frequency) / 2;
    pwm2_toggle = (pwm1_toggle * SHIFT) / 180UL;

    pwm1 = pwm1_toggle;
    pwm2 = pwm2_toggle;
    
    
  } else {
    continue; //move back
  }
}
