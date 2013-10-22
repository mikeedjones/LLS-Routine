
/*
  Simple Waveform generator with Arduino Due

 */

#include "waveform.h"
#include <Audio.h>

#define HzSample 1000000/(maxSamplesNum*75)  // sample for the 75Hz signal expressed in microseconds 

volatile int wave0 = 0;

int i = 0, j = 0;
int sample, error, p, q;
float signal[2];

//set the resolution of the pins on the board
void setup() {
  analogWriteResolution(12);  // set the analog output resolution to 12 bit (4096 levels)
  analogReadResolution(12); // set the analog input resolution to 12 bit (4096 levels), for photodiodes
}

void loop() {
  signalgen();//ceates the sine wave signal.
}

//reads the signal from the photodiode that is attached to analog pin 0. (which will 
//be measuring the probe beam) and returns an associated voltage.
float read_photodiodeprobe(){
  int pinRead0 = analogRead(A0);
  float pVolt0 = pinRead0 / 1024.0 * 3.3;
  return pVolt0;
}

//reads the signal from the photodiode that is attached to analog pin 1. 
//(which will be measuring the reference beam), and returns an associated voltage.
float read_photodioderef(){
  int pinRead1 = analogRead(A1);
  float pVolt1 = pinRead1 / 1024.0 * 3.3;
  return pVolt1;
}

//performs the differtation and phase shift calculations. Contains the XOR gate nessacary to 
//produce the error signal, which it returns as a float. 
int signal_analysis(float signal[], int i, int j){
  float absorbtion_derivative;
  if(j==1){
    absorbtion_derivative = signal[1]-signal[0];
  }
  
  if(j==0){
    absorbtion_derivative = signal[0]-signal[1];
  }
  
  if (absorbtion_derivative > 0)
    p=1;
  if (absorbtion_derivative > 0)
    p=0;
  
  q = derivativeTable[i];
  
  return ((p || q) && !(p && q));//XOR gate
}

void signalgen(){
  delayMicroseconds(HzSample);  // Hold the sample value for the sample time
  
  signal[j] = read_photodioderef()-read_photodiodeprobe();
  error = signal_analysis(signal, i, j); 
  //lasercontrol(error);
  analogWrite(DAC0, waveformsTable[i]);  // write the selected waveform on DAC0
    
  j++;
  if(j == 1)  // for derrivative calculations
    j = 0;
    
  i++;
  if(i == maxSamplesNum)  // Reset the counter to repeat the wave
    i = 0;
}
