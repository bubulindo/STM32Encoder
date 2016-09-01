#include <libmaple/timer.h>

//Encoder Direction
#define POSITIVE 	0
#define NEGATIVE	1

//Encoder Polarity
#define NORMAL 		0
#define INVERTED	1


//Angle calculation definitions
#define DEGREES		0
#define GRADES		1
#define RADIANS		2

//pin remapping
#define NO_REMAP		0
#define PARTIAL_REMAP	1
#define FULL_REMAP		3

//count modes.
#define COUNT_BOTH_CHANNELS		TIMER_SMCR_SMS_ENCODER3
#define COUNT_CH1				TIMER_SMCR_SMS_ENCODER1
#define COUNT_CH2				TIMER_SMCR_SMS_ENCODER2




#ifndef _ENCODER_H_
#define _ENCODER_H_

class STM32Encoder {
	private: 

		unsigned int _ppr; //pulses per revolution of the encoder. 
		unsigned int _prescaler; //timer prescaler. 
//		unsigned long _revolutions; //every time there is an interrupt, this variable is inc or decremented 
		timer_dev * _TIMER;
		
	public: 
	/*
	Constructor. 
	Timer - Timer to be used. On a Maple, Timer 1 through 4
	count_mode - Counting edges on both channels or just one and if so, which. 
	prescaler - Timer prescaler value. Useful to only count positive flanks if set to two. 
	pulsePerRevolution - number of pulses per Revolution on the encoder. Used for the Angle 
	calculation, interrupt trigger and reload value on the counter. 
	*/
		STM32Encoder (timer_dev * Timer, unsigned char count_mode, unsigned int prescaler, unsigned int pulsePerRevolution);
		
	/*
	Shaft Angle calculator.
	depending on where the encoder was reset and the number of pulses per revolution set,
	This function will calculate the current angle of the shaft. 
	base - units in which to present the angle. Possible Values: DEGREES, RADIANS, GRADIANS
	*/
		float getAngle(unsigned char base); //this returns the current encoder angle based on the number of counts and PPR. 
		
	/*
	Direction indicator. 
	This gives the current direction of the encoder. 
	*/
		unsigned char getDirection();
		
	/*
	This sets the polarity of the encoder. 
	Possible values: NORMAL, INVERTED
	*/
		void Polarity(unsigned char pol);
		
	/*
	Reset encoder counts. 
	*/
		void reset();
	/*
	returns an unsigned int with the current counter value. 
	*/
		unsigned int value(); //sent number of counts.
		
	/*
	Sets the prescaler of the Timer
	*/
		void setPrescaler(unsigned int prescale);
		
	/*
	Gets the prescaler of the timer. 
	*/
		unsigned int getPrescaler();
		
	/*
	Changes the pulses per revolution of the encoder. 
	This goes from 0 to 65535
	*/
		void setPPR(unsigned int pulserPerRevolution);
		
	/*
	Returns the pulses per revolution
	*/
		unsigned int getPPR();
		
	/*
	Attach an interrupt that is triggered every time the Timer passes through zero. 
	*/
		void attachInterrupt(voidFuncPtr func);
		
	/*
	Detach the interrupt.
	*/
		void detachInterrupt();
		
	/*
	Adjust the filter on the input channels. 
	This can take any value from 0x0 to 0xF. 
	0x0 means no filtering and 0xF the biggest filter. 
	*/
		void setFilter(unsigned char val);	
};

#endif