/*
 * File:   USART.c
 * Author: FDRP
 *
 * Created on 20 de Marzo de 2021, 16:40
 */
#ifndef WATCHDOG_H
#define	WATCHDOG_H
#include <xc.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>

void WATCHDOG_init();
void WATCHDOG_SetPreescaler(int n);
void WATCHDOG_TurnON();
unsigned char WATCHDOG_TurnOff();
#endif 