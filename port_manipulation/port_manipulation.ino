unsigned char urMom;


void setup() {

  pinMode(4, OUTPUT);

  //CMPnEN0 enabled,
  //waveform generation mode 101, DSTOP, Dual slope PWM, PER, BOTTOM, TOP
  TCA0 = 0b00010101

  //set high counter value
  TCA0.CNTL = 0b11111111
  TCA0.CNTH = 0b00000000

  //set compare match register value
  TCA0.CMP0L = 0b01111111
  TCA0.CMP0H = 0b00000000

  TCCR1A = 1

}

void loop() {
  // put your main code here, to run repeatedly:

}
