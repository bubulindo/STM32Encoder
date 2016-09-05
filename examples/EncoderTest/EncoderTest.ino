#include <STM32Encoder.h>
/*
For Maple Mini: 

Timer 1, channels are at 26 and 27. 
The example below was tested on an Olimexino
*/


long revolutions = 0;//revolution counter

//Encoder stuff...
STM32Encoder enc(TIMER1, COUNT_BOTH_CHANNELS, 1, 100); 

/*interrupt handler. 
  this will called every time the counter goes through zero 
  and increment/decrement the variable depending on the direction of the encoder. 
*/
void func() {  
  if (enc.getDirection() == POSITIVE) revolutions++;
  if (enc.getDirection() == NEGATIVE) revolutions--;
}

//encoder simulation stuff
unsigned char mode = 0; //clock on both pins. 
unsigned char dir = 'F';
unsigned int freq = 100;
unsigned long time = 0;
unsigned char states[4];



void setup() {

  //configure inputs for the encoder
  //comment the timer not used. 
  pinMode(D6, INPUT);  //TIMER 1 channel 1
  pinMode(D7, INPUT);  //TIMER 1 channel 2
//  pinMode(D2, INPUT);  //TIMER 2 channel 1
//  pinMode(D3, INPUT);  //TIMER 2 channel 2
//  pinMode(D12, INPUT);  //TIMER 3 channel 1
//  pinMode(D11, INPUT);  //TIMER 3 channel 2
//  pinMode(D5, INPUT);  //TIMER 4 channel 1
//  pinMode(D9, INPUT);  //TIMER 4 channel 2



  //attach interrupt
  enc.attachInterrupt(func);//attach interrupt
  
  
//setup encoder simulator  
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);

  digitalWrite(5, HIGH); 
  digitalWrite(4, LOW);
  states[0] = 0; //00
  states[1] = 1; //01
  states[2] = 3; //11
  states[3] = 2; //10



}

unsigned long interval = 0; 

void loop(){

//--------------------------------+ENCODER TEST CODE+-------------------------------    
    //encoder code
  if (millis() - interval >= 1000) { 
     Serial.print(enc.value()); 
     Serial.println(" counts");
     Serial.print("direction ");
     Serial.println(enc.getDirection());
     Serial.println(enc.getAngle(DEGREES));
     Serial.println(revolutions);
     interval = millis();
   }
  
//--------------------------------+ENCODER SIGNAL SIMULATION+-------------------------------  
//simulate encoder signals.   
//encoder simulation code  
  if (millis() - time >= freq) { 
    time = millis(); //prepare next
    
    if (dir == 'F')  mode++;
    else mode --;
    
    digitalWrite(4, (states[mode%4] & 0x01));
    digitalWrite(5, (states[mode%4] >> 1));

//--------------------------------+SERIAL COMMUNICATION+-------------------------------  
//take care of comms...
  if (Serial.available() > 0)
    switch(Serial.read()) {
      case '0': {
        freq = 100;
        break;
      }
      case '1': {
        freq = 500;
        break;
      }
      case '2': {
        freq = 1000;
        break;
      }  
      case '3': {
        freq = 50;
        break;
      }
      case 'F': {
        dir = 'F';
        break;
      }
      case 'B': {
        dir = 'B';
        break;
      }
      case 'I': {
        enc.Polarity(INVERTED);
        break;
      }
      case 'N': {
        enc.Polarity(NORMAL);
        break;
      }
      case 'R': {
        enc.reset();
        break;
      }
    }//end switch

  }

}

