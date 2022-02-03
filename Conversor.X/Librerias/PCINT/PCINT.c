/*
 * File:   PCINT.c
 * Author: FDPR
 *
 * Created on 25 de marzo de 2021, 05:29
 */

#include "PCINT.h"
#include <xc.h>
#include <avr/interrupt.h>
#include <avr/io.h>
void PCINT_init()
{
    DDRD &=~ (1<<5);
	PORTD |= (1<<5);
    // CONFIGURACIÓN DE INTERRUPCIONES
	PCICR |= (1<<2);
	PCMSK2 |= (1<<5);
}