/*
 * File:   TIMER1.c
 * Author: FDR-96
 *
 * Created on 23 de enero de 2022, 01:23
 */


#include <xc.h>
#include "TIMER1.h"
void  TIMER1_init(){

        TCNT1 =  0x00;                       //SETTING INTIAL TIMER VALUE
        TCCR1B |= (1 << ICES1);          //SETTING FIRST CAPTURE ON RISING EDGE ,(TCCR1B = TCCR1B | (1<<ICES1)
        TIMSK1 |= (1 << ICIE1) | (1 << TOIE1); //ENABLING INPUT CAPTURE AND OVERFLOW INTERRUPTS
        TCCR1B|=(0<<CS12)|(1<<CS11)|(0<<CS10);


}
