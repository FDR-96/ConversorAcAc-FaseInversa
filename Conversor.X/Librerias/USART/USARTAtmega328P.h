/*
 * File:   USART.c
 * Author: FDR-96
 *
 * Created on 21 de febrero de 2021, 19:40
 */
#ifndef USARTAtmega328P_H_
#define USARTAtmega328P_H_
#include <xc.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>


void USART_init();
void USART_SetData(unsigned char Tx);
void USART_SetArrayData(char *ArrayTx, int len);

#endif 