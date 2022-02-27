/*
 * File:   ADC.c
 * Author: FDR-96
 *

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
}

