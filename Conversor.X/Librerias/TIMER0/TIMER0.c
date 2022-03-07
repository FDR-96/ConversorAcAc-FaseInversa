/*
 * TIMER.C
 *
 * Created: 2/03/2019 16:24:04
 *  Author: Marlon
 */ 
 #define F_CPU 16000000
 #include "TIMER0.h"

 void TIMER0_init(){
	 
	 //Modo de operacion = NORMAL, OVERFLOW O SOBRECARGA
	 TCCR0A &=~ (1<<WGM00);
	 TCCR0A &=~  (1<<WGM01);
	 TCCR0B &=~ (1<<WGM02);

	 TIMSK0 |= (1<<TOIE0);// Habilitación de interrupción de desbordamiento de temporizador/contador0
     TCNT0 = 0xEC;//Iniciamos el valor del contador
	 /*
		Fint = Fcpu/(N*(Rcom-max + 1))
		1 = 16MHz/(N*256)
		N = 64 -> CS2 = 010
      * Fint = 975
	*/
     //Seleccionamos la fuente de reloj que utilizara el timer/counter
	 TCCR0B |= (0<<CS00);	
	 TCCR0B |=  (1<<CS01);	
	 TCCR0B |= (0<<CS02);	
 }
