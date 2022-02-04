
#define __DELAY_BACKWARD_COMPATIBLE__
#define F_CPU 16000000
#include <xc.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>     
#include <math.h>

//Librerias propias.
#include "Librerias/INT/INT.h"
#include "Librerias/TIMER0/TIMER0.h"
#include "Librerias/PCINT/PCINT.h"
#include "Librerias/TIMER1/TIMER1.h"
#include "Librerias/USART/USARTAtmega328P.h"

//Declaracion de variables.
const uint16_t tMax = 163; //Tiempo maximo que puede alcanzar el timer sin desbordar 
const uint16_t rComp = 256; //Registro de comparacion maximo de 8bit
const uint16_t tSemiCiclo = 100; //Tiempo de un semiciclo
volatile uint8_t tensionRed = 110;
volatile uint16_t captura = 0; //Capturara el valor del timer
volatile uint16_t ancho = 0; //Valor final del ancho de pulso
volatile uint16_t angulo = 0;

volatile uint16_t msegundos = 0;
volatile uint8_t segundos = 0;

volatile bool capturar = false;
volatile bool flanco = true;
double PI = 3.14159265358979323846;

//Declaracion de Funciones.
//Funciones exclusivas del Modo Remoto.
void ModoRemoto();
void Interfaz();
void AjustaTensionDeRedRemoto();
void AjustarTensionDimmerRemoto();
//Funciones exclusivas del Modo Local.
void ModoLocal();
void AjustaTensionDeRedRemoto();
void AjustarTensionDimmerRemoto();
//Funciones generales utilizadas por ambos modos.
void MostrarNumero(uint16_t numero, uint8_t digitos);  //Digitos = 3: xxx / Digitos = 4: xx,xx / Digitos = 5: xxx,xx
uint16_t CalcularTension();
void AjustarTension();

//Rutinas de servicio de interrupción.
ISR(INT0_vect){

   ancho = tSemiCiclo -((captura*tMax)/rComp); //157 equivalen a 10ms
     MostrarNumero(ancho, 3);
USART_SetData('s');

   //   PORTD = (1<<4);
//   _delay_ms(1000);
//   PORTD = (0<<4);
}

ISR(INT1_vect){

 //  MostrarNumero(1111, 4);
}

ISR(TIMER0_OVF_vect) {
    msegundos++;
    if(msegundos == 100){
        msegundos = 0;
        segundos++;
        if(segundos == 60){
            segundos = 0;
        }else if(segundos == 30){
            TCCR1B|=(1<<CS12)|(0<<CS11)|(1<<CS10);
        }
    }
}

//Si el TC (de 8 bits) se ejecuta a 16 MHz, se desbordará después de 16,384 ms, lo que supera con creces el intervalo de muestra recomendado de 10 ms.
//No es necesario comprobar si hay desbordamiento. Si borra el contador en el flanco ascendente, entonces el conteo en el flanco descendente es el 
//ancho del pulso en microsegundos:

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
        TCCR1B|=(0<<CS12)|(0<<CS11)|(0<<CS10);
	}
     flanco = !flanco;
}



void main() {
    cli();
        PCINT_init();
        INT_init();
        TIMER0_init();
        TIMER1_init();
        USART_init();
        DDRD = (1<<4);
        PORTD = (0<<4);
    sei();
    TIMER0_Start();
//         MostrarNumero(123, 3);
//         USART_SetData('d');
//        MostrarNumero(2232, 4);
//                 USART_SetData('x');
//        MostrarNumero(33332, 5);
    while(1){

    }
}


void MostrarNumero(uint16_t numero, uint8_t digitos)
{
	uint16_t _unidades = numero%10;
	uint16_t _decenas = (numero/10)%10;
    uint16_t _centenas = (numero/100)%10;
    uint16_t _milesu = (numero/1000)%10;
    uint16_t _milesd = (numero/10000)%10;
    if(digitos == 3){
      char _tx3[3] ={_centenas + '0',_decenas + '0', _unidades + '0'};
      USART_SetArrayData(_tx3, sizeof _tx3);
    }else if(digitos == 4){
        char _tx4[5] ={ _milesu + '0', _centenas + '0',',',_decenas + '0', _unidades + '0' };
        USART_SetArrayData(_tx4, sizeof _tx4);
    }else if(digitos == 5){
        char _tx5[6] ={_milesd + '0', _milesu + '0', _centenas + '0',',',_decenas + '0', _unidades };
        USART_SetArrayData(_tx5, sizeof _tx5);
    }else{
        char _tx2[6] ={_centenas + '0',_decenas + '0',',', _unidades + '0','m','s'};
        USART_SetArrayData(_tx2, sizeof _tx2);
    }

}

uint16_t  CalcularTension(){
    angulo = (180*ancho)/100;
    uint16_t _tensionCarga =  (tensionRed*sqrt(2)/PI)*(1+cos(angulo * PI/180))*100;
    return _tensionCarga;
}

void AjustarTension(){

}
