#include <STM32Encoder.h>

STM32Encoder::STM32Encoder (timer_dev * Timer, unsigned char count_mode, unsigned int prescaler, unsigned int pulsePerRevolution) {
	_TIMER = Timer; //Timer to be used. 
	timer_init(_TIMER);//use a general use timer.

	_prescaler = prescaler;
	(_TIMER->regs).gen->PSC = _prescaler;
	_ppr = pulsePerRevolution;
		
	//map inputs. 
	(_TIMER->regs).gen->CCMR1 = TIMER_CCMR1_CC1S_INPUT_TI1 | TIMER_CCMR1_CC2S_INPUT_TI2;
	//Count mode
	(_TIMER->regs).gen->SMCR = count_mode; //choose encoder 3, counting on both edges. 

	//set the interval used by the encoder.
	timer_set_reload(_TIMER, _ppr); //after each revolution, interrupt and increment/decrement. 

	//Go Timer, GO!!!!!!!
	timer_resume(_TIMER);
};

float STM32Encoder::getAngle(unsigned char base){

	const float angle_base[3] = {360.0f, 6.2831f, 400.0f};

	if (base <= 2){
		return ((value()/(float)_ppr) * angle_base[base]);
	} else {
		return -900.0; //ridiculous, isn't it? 
	}
		
}

unsigned char STM32Encoder::getDirection(){
	return *bb_perip(&(_TIMER->regs).gen->CR1, TIMER_CR1_DIR_BIT);
}

void STM32Encoder::Polarity(unsigned char pol) {
	if ( pol == INVERTED) {
		(_TIMER->regs).gen->CCER |= (1<<TIMER_CCER_CC1P_BIT); //to invert the counting, only one of the inputs should be inverted.  
	}else{ //pol = NORMAL
		(_TIMER->regs).gen->CCER &= ~( 1 << TIMER_CCER_CC1P_BIT);
	}
}

void STM32Encoder::reset(){
	timer_set_count(_TIMER, 0);
};

unsigned int STM32Encoder::value() {
	return timer_get_count(_TIMER); 
};
	
void STM32Encoder::setPrescaler(unsigned int prescale){
	timer_set_prescaler(_TIMER, prescale); 
};

unsigned int STM32Encoder::getPrescaler(){
	return timer_get_prescaler(_TIMER); 
};

void STM32Encoder::setPPR(unsigned int pulserPerRevolution){
	_ppr = pulserPerRevolution; 
	timer_set_reload(_TIMER, _ppr); //update counts.
};

unsigned int STM32Encoder::getPPR(){
	return _ppr;
};

void STM32Encoder::attachInterrupt(voidFuncPtr func) {

	//attach interrupt
	timer_attach_interrupt(_TIMER, TIMER_UPDATE_INTERRUPT, func);
	
	//Enable Interrupt
	(_TIMER->regs).gen->CR1  |=TIMER_CR1_UDIS_BIT;
}

void STM32Encoder::detachInterrupt() {
	(_TIMER->regs).gen->CR1  &= ~(1 << TIMER_CR1_UDIS_BIT);
	
	timer_attach_interrupt(_TIMER, TIMER_UPDATE_INTERRUPT, NULL);	
}

void STM32Encoder::setFilter(unsigned char val) {

	val = val & 0xF; //clear the upper nibble.
	(_TIMER->regs).gen->CCMR1 = ((_TIMER->regs).gen->CCMR1 & 0x0F0F) | (val << 12) | (val <<4); 
}


/*
void STM32Encoder::timer_interrupt_handler(){
	if (Direction() == POSITIVE) _revolutions++;
	if (Direction() == NEGATIVE) _revolutions--;
}*/


/*
*  Remap Inputs
*  The table below indicates the pins that can be used with the remap method. 
*  TIMER3 is a special case as the partial remap can use completely different pins than no remap and full remap. 
*  All other timers either use the same pins (mind you the encoder only uses 2 channels) or don't have the possibility 
*  of partial remap. 
*  
*  Timer Remapping Table. 
*  TIMER1 - NORMAL - CH1 = PA8(D6), CH2 = PA9(D7) / FULL - CH1 = PE9, CH2 = PE11  
*  TIMER2 - NORMAL - CH1 = PA0(D2), CH2 = PA1(D3) / FULL - CH1 = PA15(D41), CH2 = PB3(D42) 
*  TIMER3 - NORMAL - CH1 = PA6(D12), CH2 = PA7(D11) / FULL - CH1 = PC6(D35), CH2 = PC7(D36) / PARTIAL - CH1 = PB4(D43), CH2 = PB5(D4)  
*  TIMER4 - NORMAL - CH1 = PB6(D5), CH2 = PB7(D9) / FULL - CH1 = PD12, CH2 = PD13
*/

/*
void STM32Encoder::remapInputs(unsigned char val){  //??? Is it worth it? 
//if this function is called, then we need to start the AFIO
	afio_init(); 
	unsigned char shift = 0; 

	switch(_TIMER) {
		case TIMER1: { //does this work???? 
			shift = 6;
			break;
		}
		case TIMER2: {
			shift = 8;
			break;
		}
		case TIMER3: {
			shift = 10; 
			break;
		}
		case TIMER4: {
			//hmmm... bit 12
			shift = 12;
			val = val % 1; //this is a bit, so we can only shift 1 or 0, not 3.
			break;
		}
		//TO DO: Support for Maple Native. 
	} //end switch
afio_remap( (val << shift));

}


}
*/
