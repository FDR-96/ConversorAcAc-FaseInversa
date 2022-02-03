/*
 * File:   WatchDog.c
 * Author: FDPR
 * Created on 20 de Marzo de 2021, 16:40
 */

#include "WATCHDOG.h"
#include <xc.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>
#define WDTO_15MS 0
#define WDTO_30MS 1
#define WDTO_60MS 2
#define WDTO_120MS 3
#define WDTO_250MS 4
#define WDTO_500MS 5
#define WDTO_1S 6
#define WDTO_2S 7
#define WDTO_4S 8
#define WDTO_8S 9


void WATCHDOG_init(){
    wdt_reset();
    //set up WDT interrupt
    WDTCSR = (1<<WDCE)|(1<<WDE);
    
}
void WATCHDOG_SetPreescaler(int n){
    WDTCSR = (1<<WDIE)|(1<<WDE)|(1<<WDP0); //Sistem reset mode en 4k ciclos, equivalentes a 32ms
}

