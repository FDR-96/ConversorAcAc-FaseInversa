/*
 * File:   main.c
 * Author: FDR-96
 *
 * Created on 23 de enero de 2022, 01:59
 */


#define F_CPU 16000000
#include <xc.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//Librerias propias.
#include "Librerias/PCINT/PCINT.h"
#include "Librerias/TIMER1/TIMER1.h"
#include "Librerias/USART/USARTAtmega328P.h"

//Declaracion de variables.
const uint16_t tMax = 163; //Tiempo maximo que puede alcanzar el timer sin desbordar 
const uint16_t rComp = 256; //Registro de comparacion maximo de 8bit
const uint16_t tSemiCiclo = 100; //Tiempo de un semiciclo
volatile uint16_t tensionRed = 110;
volatile uint16_t captura = 0; //Capturara el valor del timer
volatile uint16_t ancho = 0; //Valor final del ancho de pulso
volatile uint32_t angulo = 0;
bool capturar = false;
bool flanco = true;
double PI = 3.14159265358979323846;

//Declaracion de Funciones.
void DividirMostrarNumero(uint16_t numero);
int CalcularTension();
void AjustarTension();

//Interrupciones.
ISR(INT0_vect){
   ancho = tSemiCiclo -((captura*tMax)/rComp); //157 equivalen a 10ms
   DividirMostrarNumero(CalcularTension());
}

ISR(TIMER1_CAPT_vect) {
  if(flanco) // Detectamos un flanco ascendente
	{
		TCCR1B &=~ (1 << ICES1); //cambiamos el sensado establecido por el de descendente
		TCNT1 = 0x00;
	}
  else // Detectamos un flanco descendente
	{
		captura = ICR1; // record time at which falling edge occurs
		TCCR1B |= (1 << ICES1); //cambiamos el sensado establecido por el de ascendente
	}
     flanco = !flanco;
}

ISR(USART_RX_vect){
	USART_GetData();
}

void main() {
    cli();
        PCINT_init();
        TIMER1_init();
        USART_init();
    sei();
    
    while(1){}
}


void DividirMostrarNumero(uint16_t numero)
{
	uint8_t _unidades = numero%10;
	uint8_t _decenas = (numero/10)%10;
    uint8_t _centenas = (numero/100)%10;
    char _tx16[3] ={_centenas + '0',_decenas + '0', _unidades + '0'};
    USART_SetArrayData(_tx16, sizeof _tx16);
}

int CalcularTension(){
    angulo = (180*ancho)/10;
    uint32_t _tensionCarga =  (tensionRed*sqrt(2)/PI)*(1+cos(angulo * PI/180))*10;
    return _tensionCarga;
}

void AjustarTension(){}