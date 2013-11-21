/*
  Simple Waveform generator with Arduino Due

 */


#include "waveform.h"

#define encoder0PinA 24
#define encoder0PinB 22

volatile int wave0 = 0;

int i = 0, j = 0;
int sample, error, p, q;
float p_dsignal[2];
long DCoffset = 0;


void setup() {
  //set the resolution of the pins on the board
  Serial.begin(9600);
  Serial.print("hello");
  analogWriteResolution(12);  // set the analog output resolution to 12 bit (4096 levels)
  analogReadResolution(12); // set the analog input resolution to 12 bit (4096 levels), for photodiodes
  Serial.print("hello2");
 // turn on pullup resistors
  digitalWrite(encoder0PinA, HIGH);
  digitalWrite(encoder0PinB, HIGH);
  Serial.print("hello3");
  attachInterrupt(encoder0PinA, changeDCoffsetup, FALLING);  // encoder on pin 24
  Serial.print("hello4");
  pinMode(50, OUTPUT);
}

void loop() {
  signalgen();//ceates the sine wave signal.
}

//reads the signal from the photodiode that is attached to analog pin 0. (which will 
//be measuring the probe beam) and returns an associated voltage.
float read_photodiodeprobe(){
  int pinRead0 = analogRead(A0);
  float pVolt0 = pinRead0 * 0.0008057;
  return pVolt0;
}

//reads the signal from the photodiode that is attached to analog pin 1. 
//(which will be measuring the reference beam), and returns an associated voltage.
float read_photodioderef(){
  int pinRead1 = analogRead(A1);
  float pVolt1 = pinRead1 * 0.0008057;
  return pVolt1;
}

//performs the differtation and phase shift calculations. Contains the XOR gate nessacary to 
//produce the error signal, which it returns as a float. 
int signal_analysis(float signal[], int i, int j){
  float absorbtion_derivative;
  if(j==1){
    absorbtion_derivative = p_dsignal[1]-p_dsignal[0];
  }
  
  if(j==0){
    absorbtion_derivative = p_dsignal[0]-p_dsignal[1];
  }
  
  if (absorbtion_derivative > 0)
    p=1;
  if (absorbtion_derivative > 0)
    p=0;
  
  q = derivativeTable[i];
  
  return ((p | q) & ~(p & q));//XOR gate
}

void lasercontrol(int error){
  digitalWrite(50, error);
}

void changeDCoffsetup(){  
  byte A = digitalRead(encoder0PinA);
  byte B = digitalRead(encoder0PinB);
  
  if(B & ~A & (DCoffset > -1024)){
    DCoffset--;
  }
  
  else if(~A & ~B & (DCoffset < +1024 )){
    DCoffset++;
  }
  Serial.println(DCoffset);
}
  
void signalgen(){
  //delayMicroseconds(HzSample);  // Hold the sample value for the sample time 
  p_dsignal[j] = read_photodioderef()-read_photodiodeprobe();
  error = signal_analysis(p_dsignal, i, j); 
  //lasercontrol(error);
  analogWrite(DAC1, waveformsTable[i]+DCoffset);  // write the selected waveform on DAC1
  
  j++;
  if(j == 1)  // for derrivative calculations
    j = 0;
    
  i++;
  if(i == maxSamplesNum)  // Reset the counter to repeat the wave
    i = 0;
}
