#include <Wire.h>
#include <PWM.h>

#define address 0x36 //define slave address, Q: need to <<1 ?  

//enable pin
int enablePin = 0;

// millisecond counters
long us = 0;
long us2 = 0;

bool ledState = 0;
bool ledState2 = 0;

bool enable = 0; // 1 bit

long elapsed = 0;
long elapsed2 = 0;

//us per cycle
int frequency = 0;
long threshold = 10;
long buffer = 0;

int msg = -1;

void setup() {
  
  Wire.begin(address);

  pinMode(enablePin, OUTPUT); //enable 
  pinMode(4, OUTPUT); // pwm1
  pinMode(1, OUTPUT); // pwm2 

  Wire.onReceive(parseMsg); //call parseMsg when message is received
}

void parseMsg(int bytesLeft) {
  
  msg = 0; //reset as 0 everytime msg is received

 //Wire.available() returns 0 initially because master checks validity of slave 
 //actually value comes after
 if (Wire.available() != 0) {
   msg = Wire.read(); //read one byte from buffer. Message should only be one byte.
 }

 switch (msg) {
    case 0:
      createPWM(false, 0);
      break;
    case 1:
      createPWM(false, 0);
      break;
    case 2:
      createPWM(false, 1250);
      break;
    case 3:
      createPWM(false, 1250);
      break;
    case 4:
      createPWM(false, 2500);
      break;
    case 5:
      createPWM(false, 2500);
      break;
    case 6:
      createPWM(false, 3750);
      break;
    case 7:
      createPWM(false, 3750);
      break;
    case 8:
      createPWM(true, 0);
      break;
    case 9:
      createPWM(true, 0);
      break;
    case 10:
      createPWM(true, 1250);
      break;
    case 11:
      createPWM(true, 1250);
      break;
    case 12:
      createPWM(true, 2500);
      break;
    case 13:
      createPWM(true, 2500);
      break;
    case 14:
      createPWM(true, 3750);
      break;
    case 15:
      createPWM(true, 3750);
      break;
    default: //else do nothing; will be contacted through initial bus scan
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
  
  if (frequency == 0) { //avoid divide by zero
    threshold = 0;
    buffer = 0;
  }

  else {
    threshold = (1000000 / frequency);
    buffer = threshold/5; // possible error with division? 
  }
  
}

void loop() {

  us = micros();
  us2 = us + threshold/2;

  if (us % threshold < buffer && us - elapsed > buffer) {
    digitalWrite(4, !ledState); 
    ledState = !ledState;
    elapsed = us;   
  }

  if (us2 % threshold < buffer && us2 - elapsed2 > buffer) {
    digitalWrite(1, !ledState2);
    ledState2 = !ledState2;
    elapsed2 = us2;
  }

}