/*
 * File:   USARTAtmega328P.c
 * Author: FDR-96
 *
 * Created on 21 de febrero de 2021, 19:44
 */



#define F_CPU 16000000
#include "USARTAtmega328P.h"
#include <xc.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <util/delay.h>
#define BAUD_RATE_9600_BPS  71  // 9600bps
#define BAUD_RATE_115200_BPS  5 // 115.2k bps
#define FOSC 16000000
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1 // modo normal
void USART_init()
{
    unsigned int ubrr = MYUBRR ;
    UBRR0H = ((unsigned int)ubrr>>8);
    UBRR0L = ((unsigned int)ubrr);  
    UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1 << RXCIE0);; //habilitamos el receptor y el trasmisor y habilitamos la interrupcion por recepcion completa 
    UCSR0C = (1<<USBS0)|(2<<UCSZ00); //2bit stop, seleccionamos el tamaño del caracter que vamos a recibir, en nuestro caso  8bit

}


void USART_SetData(unsigned char Tx)
{
     while (!( UCSR0A & (1<<UDRE0))); //Esperamos a que sea 1, lo que significa que el registro esta libre para ser utilizado.
     UDR0 = Tx; //cargamos el buffer o el registro en el que se almacena la informacion a ser trasmitida
     
}

void USART_SetArrayData(char *ArrayTx, int len)
{
    for (int i = 0; i < len; ++i) {
       while (!( UCSR0A & (1<<UDRE0)));
        UDR0 = ArrayTx[i];
     }
}
