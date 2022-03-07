/*
 * File:   TIMER1.c
 * Author: FDR-96
 *
 * Created on 23 de enero de 2022, 01:23
 */


#include <xc.h>
#include "TIMER1.h"
void  TIMER1_init(){
        TCNT1 =  0x00;                       //Configuramos el valor inicial del contador
        TCCR1B |= (1 << ICES1);          //Configuramos el flanco de captura como ascendente
        TIMSK1 |= (1 << ICIE1) | (1 << TOIE1); //Habilitamos la entrada de captura y el interruptor de overflow
        TCCR1B |=(0<<CS12)|(1<<CS11)|(0<<CS10); //Seleccionamos la fuente de reloj que utilizara el timer/counter
}
