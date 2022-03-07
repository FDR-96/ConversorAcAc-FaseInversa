/*
 * File:   ADC.c
 * Author: FDR-96
 *

 */


#define F_CPU 16000000

#include "ADC.h"

void ADC_init()
{
    ADMUX |= (0<<REFS1)|(1<<REFS0)|(0<<ADLAR); //Registro de selección de multiplexor ADC. AVCC con capacitor externo en pin AREF. Sin resultado de ajuste izquierdo de ADC
    ADMUX |=  (1<<MUX0); //Encendemos el canal 1
    ADCSRA |= (1<<ADEN) | (1<<ADATE) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);//ADCSRA: registro A de control y estado del ADC
    //ADEN: Habilita  el ADC.
    //ADATE: Habilitar activación automática de ADC. 
    //ADIE: Habilitar interrupción ADC.
    //ADPSx: Bits de selección del preescalador ADC, en este caso 128.
    ADCSRB |= (1<<ADTS2)|(1<<ADTS1)|(1<<ADTS0); //Establecer fuente de activación ADC - Timer/Counter1 capture event
    ADCSRA |= (1<<ADSC); //Comenzamos la convercion
      return;
}

