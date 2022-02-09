/*
 * File:   TIMER1.c
 * Author: FDR-96
 *
 * Created on 23 de enero de 2022, 01:23
 */


#include <xc.h>
#include "TIMER1.h"
void  TIMER1_init(){
        TCNT1 =  0x00; //CONFIGURAR EL VALOR DEL TEMPORIZADOR INICIAL
        TCCR1B |= (1 << ICES1);    //CONFIGURAR LA PRIMERA CAPTURA EN BORDE ASCENDENTE,(TCCR1B = TCCR1B | (1<<ICES1)
        TIMSK1 |= (1 << ICIE1) | (1 << TOIE1); //HABILITAR LA CAPTURA DE ENTRADA Y LAS INTERRUPCIONES DE DESBORDAMIENTO
        TCCR1B|=(1<<CS12)|(0<<CS11)|(1<<CS10);
}
