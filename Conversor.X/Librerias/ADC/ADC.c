/*
 * File:   ADC.c
 * Author: FDR-96
 *
<<<<<<< HEAD
 */


#define F_CPU 16000000

#include "ADC.h"
#include <util/delay.h>
void ADC_init()
{
    ADMUX |= (1<<REFS0)|(1<<ADLAR);
    ADMUX |=  (1<<MUX0);
    ADCSRA |= (1<<ADEN) | (1<<ADATE) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	
    //set ADC trigger source - Timer0 compare match A
    ADCSRB |= (1<<ADTS2)|(1<<ADTS1)|(1<<ADTS0);

    // StartADC
    ADCSRA |= (1<<ADSC); 

      return;
//	// Output adjust = right //
//	ADMUX = (0<<ADLAR);
//
//	// Voltage Reference = AVCC //
//	ADMUX =  (1<<REFS0)|(0<<REFS1);
//
//	// Frequency divisor = 128 -> 16000/128 = 125 KHz
//	ADCSRA = (1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);
}

volatile uint16_t ADC_GetData(int canal)
{
	// Seleccion del canal de lADC //
	ADMUX &=~  0x0F;
	ADMUX |=  canal;
	
	// Encendemos en ADC
	ADCSRA |= (1<<ADEN);
	_delay_us(10);	// Esperamos a que caliente

	// Mandamos el muestreo
	ADCSRA |= (1<<ADSC);

	// Esperamos a que muestree, leyendo el flag
	while( !(ADCSRA & (1<<ADIF)) );
	ADCSRA |= (1<<ADIF);	// Reiniciamos el flag

	// Apagamos el ADC
	ADCSRA = (0<<ADEN);

	return ADC;	
}
