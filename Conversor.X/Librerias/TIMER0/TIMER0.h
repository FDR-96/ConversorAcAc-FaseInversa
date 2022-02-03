/*
 * TIMER.h
 *
 * Created: 2/03/2019 16:24:16
 *  Author: marlon
 */ 


#ifndef TIMER0_H_
#define TIMER0_H_
	
	#include <avr/io.h>

	void TIMER0_init();

	void TIMER0_Start();
	void TIMER0_Stop();

#endif /* TIMER0_H_ */