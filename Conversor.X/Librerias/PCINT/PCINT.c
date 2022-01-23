/*
 * File:   PCINT.c
 * Author: FDPR
 *
 * Created on 25 de marzo de 2021, 05:29
 */

#include "PCINT.h"
#include <xc.h>

void PCINT_init()
{
    PORTD |= (1<<PORTD2);
    EICRA |= (1<<ISC00)|(1<<ISC01); // El flanco ascendente de INT1 genera una solicitud de interrupción.
    EIMSK |= (1<<INT0);
}