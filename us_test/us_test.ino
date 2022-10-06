#include <Wire.h>
#include <PWM.h>

#define CLK 16000000UL // 20 mHz or 16 mHz, figure out CLK 
#define shift 90
#define PRESCALER 1 //prescaler is a var that scales the timer speed

#define address 0x36 //define slave address, Q: need to <<1 ?  

//pins for PWM, from circuit diagram
int pwm1 = 4;
int pwm2 = 1;

//enable pin
int enablePin = 0;

// millisecond counters
long us = 0;
long ms2 = 0;
int curMs = 0;


unsigned long currentTime = 0;
unsigned long previousTime = 0;
const unsigned long eventInterval = 1000;




bool ledState = 0;
bool ledState2 = 0;

// unsigned long pwm1_toggle = null;
// unsigned long pwm2_toggle = null;

//incoming msg from master contains 4 bits
//long msg = null;

bool enable = 0; // 1 bit
int freqOpt = 0; // 2 bits {0: 0 kHz, 1: 167 kHz, 2: 333 kHz, 500 kHz} 
bool wave = 0; // 1 bit {true: step wave, false: DC performance wave}

int frequency = 0;



long remainder = 999999999;
long elapsed = 0;

void setup() {
  
  Wire.begin(address);

  pinMode(enablePin, OUTPUT); //enable 
  pinMode(4, OUTPUT); // pwm1
  pinMode(5, OUTPUT); // pwm2 

  digitalWrite(pwm1, HIGH); 
  //digitalWrite(pwm2, HIGH);
  
}

void blink() {
  ledState = !ledState;
  digitalWrite(pwm1, ledState);
}

void parseMsg(int msg) {
  
  // enable = msg[0]
  // freqOpt = strtol(msg[1:2], NULL, 2); //receive a standard i2c message as length of 8 bytes, parse that not a string 
  // //check if first four bits and last four bits are equal and if message is garbled then ignore
  // wave = msg[3]

 switch (msg) { //need to do wave type thing
    case 0:
      createPWM(false, 0);
      break;
    case 1:
      createPWM(false, 0);
      break;
    case 2:
      createPWM(false, 167);
      break;
    case 3:
      createPWM(false, 167);
      break;
    case 4:
      createPWM(false, 333);
      break;
    case 5:
      createPWM(false, 333);
      break;
    case 6:
      createPWM(false, 487);
      break;
    case 7:
      createPWM(false, 487);
      break;
    case 8:
      createPWM(true, 0);
      break;
    case 9:
      createPWM(true, 0);
      break;
    case 10:
      createPWM(true, 167);
      break;
    case 11:
      createPWM(true, 167);
      break;
    case 12:
      createPWM(true, 333);
      break;
    case 13:
      createPWM(true, 333);
      break;
    case 14:
      createPWM(true, 487);
      break;
    case 15:
      createPWM(true, 487);
      break;
    default: //else do nothing
      createPWM(false, 0);
      break;

    return;
   
 }
 
}

void createPWM(bool enable, int frequency) { //set frequencies

  if (enable) {
    digitalWrite(enablePin, HIGH);
  } else {
    digitalWrite(enablePin, LOW);
  }
  
  //SetPinFrequencySafe(pwm1, frequency); //set freq pwm1
  //SetPinFrequencySafe(pwm2, frequency); //set freq pwm2


}

void loop() {

  // currentTime = millis();

  // if(currentTime - previousTime >= eventInterval) {
  //   digitalWrite(4, !ledState);
  // }

  // previousTime = currentTime;
  
  int msg = Wire.read();
  parseMsg(msg);

  
  //ms2 = ms + 500;

  us = micros();
  if (us % 1000000 < 200000 && us - elapsed > 200000) {
    digitalWrite(4, !ledState); 
    ledState = !ledState;
    elapsed = us;   
  }

  
  //remainder = us % 1000000;

   

  // if (abs(ms2 % 1000000) < 200000) {
  //   digitalWrite(1, !ledState2);
  // }
  
  // ledState2 = !ledState2;

  // //PWM2 starts with 90 degrees phase shift from PWM1
  // //frequency / 1000 = cycles/ms 
  // //frequency / 4000 = fourth of a cycle/ms 
  // //once current milliseconds tracker has passed 1/4 of cycle, start pwm2 (90 degrees phase shift)

  // // if (ms >= frequency/4000) {  
  // //   digitalWrite(pwm2, HIGH);
  // // }

  // // if (ms >= frequency/2000) {
  // //   digitalWrite(pwm1, LOW);
  // // }

  // // if (ms >= (3 * frequency)/4000) {
  // //   digitalWrite(pwm2, LOW);
  // // }
  
  // if (enable == '1') {

  //   //pwmWrite(pwm1, 127) //50% duty cycle pwm1
  //   //pwmWrite(pwm2, 127) //50% duty cycle pwm2
  //   //TODO phase shift

  //   pwm1_toggle = (CLK / frequency) / 2;

  //   ICR1 = pwm1_toggle
    
  //   pwm2_toggle = (pwm1_toggle * SHIFT) / 180UL;

  //   OCR1B = pwm2_toggle;
    
  //  // pwm1 = pwm1_toggle;
  //  // pwm2 = pwm2_toggle;

  //   TCCR1B |= _BV(CS10); //change bit value of cs10 (clock
    
}