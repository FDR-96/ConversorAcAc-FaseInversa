/*
 * TIMER.C
 *
 * Created: 2/03/2019 16:24:04
 *  Author: Marlon
 */ 
 #define F_CPU 16000000
 #include "TIMER0.h"

 void TIMER0_init(){
	 
	 //Operation Mode = NORMAL, OVERFLOW O SOBRECARGA
	 TCCR0A &=~ (1<<WGM00);
	 TCCR0A &=~  (1<<WGM01);
	 TCCR0B &=~ (1<<WGM02);
	 
	 //enable interrupt
	 TIMSK0 |= (1<<TOIE0);
 }

 void TIMER0_Start(){

	 TCNT0 = 0x6;
	 /*
		Fint = Fcpu/(N*(Rcom-max + 1))
		1 = 16MHz/(N*256)
		N = 64 -> CS2 = 010
      * Fint = 975
	*/
	 TCCR0B |= (1<<CS00);	// 1
	 TCCR0B |=  (1<<CS01);	// 0
	 TCCR0B |= (0<<CS02);	// 1
	 
 }

 void TIMER0_Stop(){
	 
	 //Clock setting T1clock = 0 Hz
	 TCCR0B &=~ (1<<CS00);
	 TCCR0B &=~ (1<<CS01);
	 TCCR0B &=~ (1<<CS02);

 }