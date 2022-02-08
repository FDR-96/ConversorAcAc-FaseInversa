/*
 * File:   PCINT.c
 * Author: FDPR
 *
 * Created on 25 de marzo de 2021, 05:29
 */

#include "INT.h"
#include <xc.h>

void INT_init()
{
    PORTD |= (1<<PORTD2)|(1<<PORTD3);
    EICRA |= (1<<ISC00)|(0<<ISC01); // El flanco ascendente de INT0 genera una solicitud de interrupción.
    EICRA |= (1<<ISC10)|(1<<ISC11); // El flanco ascendente de INT1 genera una solicitud de interrupción.
    EIMSK |= (1<<INT0)|(1<<INT1);
}