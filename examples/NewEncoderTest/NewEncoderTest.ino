/*
For testing this library, wire D4 and D5 to D10 and D11 on your maple mini. 
You can vary the direction by swapping these connections.
*/
#include <STM32Encoder.h>

//Variables for the encoder
//prescaler must be between 1 and 2^16 
//a prescaler of 2 will get half the counts.
STM32Encoder rudder(TIMER2, COUNT_BOTH_CHANNELS, 1, 1024);
unsigned long turns = 0; //this can count the number of turns the encoder gives. 

void countTurns() {
  if (rudder.getDirection() == 1)
    turns ++; 
  else
    turns--;
  }

//Serial Port Variables
unsigned long tprint = 0; 

//Simulation variables
unsigned int freq = 250;
unsigned long tim = 0;
char dir = 'C';
unsigned int mode = 0; 
unsigned char states[4];


void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);

//input for the Timer channels
  pinMode(D10, INPUT_PULLUP);  //channel 1
  pinMode(D11, INPUT_PULLUP);  //channel 2

//attach Interrupt to count the overflows/turns
rudder.attachInterrupt(countTurns);

//Simulation setup
  states[0] = 0; //00
  states[1] = 1; //01
  states[2] = 3; //11
  states[3] = 2; //10
//Simulator outputs  
  pinMode(D4, OUTPUT);  //channel 1
  pinMode(D5, OUTPUT);  //channel 2
}


void loop() {
  //Report what you are doing...
  if (millis() - tprint > 1000) {
    tprint= millis();
    Serial.print("Counts: "); 
    Serial.println(rudder.value());
    Serial.print("direction: ");
    Serial.println(rudder.getDirection());
    Serial.print("turns: ");
    Serial.println(turns);
  }
//simulate encoder signals.   
  if ( millis() - tim >= freq) { 
    tim = millis(); //prepare next
    
    if (dir == 'F')  mode++;
    else mode --;
    
    digitalWrite(D4, (states[mode%4] & 0x01));
    digitalWrite(D5, (states[mode%4] >> 1));
  }

}
