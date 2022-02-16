
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
#include "Librerias/EEPROM/EEPROM.h"

//Librerias propias.
#include "Librerias/INT/INT.h"
#include "Librerias/TIMER0/TIMER0.h"
#include "Librerias/TIMER1/TIMER1.h"
#include "Librerias/ADC/ADC.h"
#include "Librerias/USART/USARTAtmega328P.h"
#include "Librerias/MAX7219/Max7219.h"


//Declaracion de variables.
const uint32_t tMax = 3276; //Tiempo maximo que puede alcanzar el timer sin desbordar 
const uint32_t rComp = 65536; //Registro de comparacion maximo de 8bit
const uint16_t tSemiCiclo = 100; //Tiempo de un semiciclo
volatile uint16_t tensionRed = 110;
uint8_t setPoint = 10;



volatile uint16_t captura = 0; //Capturara el valor del timer
volatile uint32_t ancho = 0; //Valor final del ancho de pulso
volatile uint8_t angulo = 0;
volatile uint16_t usegundos = 500;
volatile uint8_t porciento = 0;
volatile uint16_t ton = 0;
volatile uint16_t ema =  0;
volatile uint16_t ema1 =  0;
float alpha = 0.05;
int8_t opcion = 0;


volatile uint8_t feedback = 0;    

volatile uint8_t err = 0;



volatile uint16_t msSemiCiclo = 0; //0-1000
volatile uint8_t seg = 0; //0-100



unsigned char rx = 'o';
unsigned char tx = '+'; 

int8_t contChar = 0;

const double PI = 3.14159265358979323846;

volatile bool dimmer = true;
volatile bool click = false;
volatile bool capturar = true;



volatile bool cambio = true;
//bool buffer = false;


//Declaracion de Funciones.
//Funciones exclusivas del Modo Remoto.
void ModoRemoto();
void Interfaz(int8_t opciones);
void AjustaTensionDeRedRemoto();
void AjustarTensionDimmerRemoto();
//Funciones exclusivas del Modo Local.
void ModoLocal();
void AjustaTensionDeRedLocal();
void AjustarTensionDimmerLocal();
//Funciones generales utilizadas por ambos modos.
void MostrarNumero(uint16_t numero, uint8_t digitos);  //Digitos = 3: xxx / Digitos = 4: xx,xx / Digitos = 5: xxx,xx
void ReajustarTension();
uint16_t CalcularTensionRMS();
 void  HallarDisparo();
//Rutinas de servicio de interrupci?n.
ISR(INT0_vect){
     opcion++;
}

ISR(INT1_vect){
        TCNT0 = 0xEC;
        PORTD = (1<<7);
        msSemiCiclo = 0;
        seg++;
 }


ISR(TIMER0_OVF_vect) {

    msSemiCiclo++;  
    if(msSemiCiclo >= ton){
        PORTD = (0<<7);
    }
 
    TCNT0 = 0xec;
}

//Si el TC (de 16 bits) se ejecuta a 16 MHz, se desbordar? despu?s de 32,768 ms, lo que supera con creces el intervalo de muestra recomendado de 10 ms.
//No es necesario comprobar si hay desbordamiento. Si borra el contador en el flanco ascendente, entonces el conteo en el flanco descendente es el 
//ancho del pulso en microsegundos:

ISR(TIMER1_CAPT_vect) {
  if(capturar) // Detectamos un flanco ascendente
	{
		TCCR1B &=~ (1 << ICES1); //cambiamos el sensado establecido por el de descendente
		TCNT1 = 0x00;
	}
  else // Detectamos un flanco descendente
	{
		captura = ICR1; // record time at which falling edge occurs
        ancho = ((captura*tMax)/rComp);
		TCCR1B |= (1 << ICES1); //cambiamos el sensado establecido por el de ascendente
	}
  capturar = !capturar;
}

 ISR(ADC_vect)
 {    
     switch(opcion){
         case 0:
                ema = (0.03*ADC)+((1-0.03)*ema);
                tensionRed = (ema * 240)/1024; 
                opcion++;
             break;
         case 1:
               opcion--;
             break;
         case 2:
                ema1 = (alpha*ADC)+((1-alpha)*ema1);
                porciento = ((ema1 * 100)/1024);
                setPoint = tensionRed*porciento/100;  
                MAX7219_displayNumber(setPoint);
                cli();
                    ADMUX |=  (0<<MUX0);
                sei();
                opcion++;
             break;
          case 3:
                ema = (alpha*ADC)+((1-alpha)*ema);
                tensionRed = (ema * 240)/1024; 
                cli();
                    ADMUX |=  (1<<MUX0);
                sei();
                opcion--;
             break;
         default:
             opcion = 0;
             break;
     
     }
     
 }
 
 ISR(USART_RX_vect){
    rx = UDR0;	
    //implementamos case ya que es mas veloz
    switch(rx){
        case '¡':
            contChar = 1;
            setPoint = 0;
            break;
        case '<':
       
           break;
        case 'o':
            USART_SetData('+');
            cli();
            EIMSK = (1<<INT1);
            sei();
            break;
        case 'x':
            USART_SetData('-');
            cli();
            EIMSK = (0<<INT1);
            sei();
            break;
        default:   
            setPoint = setPoint * contChar + (rx - '0');
            contChar = contChar*10;
            break;
    }
}
 
void main() {
    cli();
        INT_init();
        TIMER0_init();
        TIMER1_init();
        ADC_init();
        USART_init();
        MAX7219_init();
        DDRD = (1<<7)|(1<<6);
        PORTD = (0<<7)|(0<<6);
    sei();
    err = EEPROM_read(96, &setPoint);
 
    while(1){

        if(seg == 20){
            angulo = (180*ancho)/1000;
            feedback =  CalcularTensionRMS();
            ReajustarTension();
            MostrarNumero(feedback, 1);
            MostrarNumero(tensionRed, 2);
            MostrarNumero(ancho, 3);
            MostrarNumero(setPoint, 4);
            MostrarNumero(angulo, 5);
            USART_SetData(tx);
            MAX7219_displayNumber(feedback);
            err = EEPROM_update(96, setPoint);
            seg = 0;
            
        }
    }
}

void MostrarNumero(uint16_t numero, uint8_t digitos){
	uint16_t _unidades = numero%10;
	uint16_t _decenas = (numero/10)%10;
    uint16_t _centenas = (numero/100)%10;  
    
    if(digitos == 1){
      char _tx1[4] ={'$', _centenas + '0', _decenas + '0', _unidades + '0' };
      USART_SetArrayData(_tx1, sizeof _tx1);
    }else if(digitos == 2){
      char _tx2[4] ={'%',_centenas + '0',_decenas + '0', _unidades + '0'};
      USART_SetArrayData(_tx2, sizeof _tx2);
    }else if(digitos == 3){
        char _tx3[4] ={ '&', _centenas + '0',_decenas + '0', _unidades + '0' };
        USART_SetArrayData(_tx3, sizeof _tx3);
    }else if(digitos == 4){
        char _tx4[4] ={'@', _centenas + '0',_decenas + '0', _unidades + '0'};
        USART_SetArrayData(_tx4, sizeof _tx4);
    }else if(digitos == 5){
        char _tx5[5] ={'*', _centenas + '0',_decenas + '0', _unidades + '0'};
        USART_SetArrayData(_tx5, sizeof _tx5);
    }
}

uint16_t  CalcularTensionRMS(){
    uint16_t _tensionRMS =  ((tensionRed*sqrt(PI)*sqrt(2)*sqrt(2*(angulo*PI/180)-sin(2*(angulo*PI/180))))/(2*PI));
    return _tensionRMS;
}
void ReajustarTension(){
      int error = setPoint-feedback;
     
      if(fabs(error)!= 0 && tensionRed > feedback + 1 ){
        int diff = 1;
        if(error > 1){
            diff = -1;
        }
        ton  =  ton + diff + error * 2;
      }else if (tensionRed == feedback + 1 && tensionRed > setPoint){
          ton = ton - 20;
      }
}