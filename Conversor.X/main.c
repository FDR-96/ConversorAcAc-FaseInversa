#include <xc.h>
#include <avr/io.h>
#include <avr/interrupt.h>
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
//MACROS
#define F_CPU 16000000
#define TIEMPO_MAXIMO_TIMER1_CAPTURE 3276
#define REGISTRO_DE_COMPARACION_TIMER1 65536
#define PI 3.14159265358979323846
#define ALPHA 0.05
#define PORCENTAJE(a) a*100/1024 
#define RAIZ_2 1.414213562
#define RAIZ_PI 1.772453851
#define PI_2 6.283185307
//Declaracion de variables.

volatile uint32_t ancho, ancho_anterior, ancho_ok; //Valor final del ancho de pulso
volatile uint16_t ms_semi_ciclo, ton, ema_a1, ema_a0; //0-1000
volatile uint8_t seg, angulo, tension_red, set_point, feedback, x_porciento; //0-100
volatile unsigned char rx = 'o';

int8_t opcion, contador_rx_isr, err;
uint8_t flanco = 1;
uint8_t cont = 0;


//Declaracion de Funciones.
void AjustarTensionDimmerLocal();//Funciones exclusivas del Modo Local.
//Funciones generales utilizadas por ambos modos.
void MostrarNumero(uint16_t numero, uint8_t digitos);  //Digitos = 3: xxx / Digitos = 4: xx,xx / Digitos = 5: xxx,xx
void ReajustarTension();
uint16_t CalcularTensionRMS(); 
//Rutinas de servicio de interrupci?n.
ISR(INT0_vect){
    opcion++;
}
ISR(INT1_vect){
    TCNT0 = 0xEC;
    PORTD = (1<<7);
    ms_semi_ciclo = 0;
    seg++;
    cont++;
 }
//Las rutinas del timer se ejecutan cada 10us
ISR(TIMER0_OVF_vect) 
{
    ms_semi_ciclo++;  
    if(ms_semi_ciclo >= ton){
        PORTD = (0<<7);
    }
    TCNT0 = 0xEC;
}
//Si el TC (de 16 bits) se ejecuta a 16 MHz, se desbordar? despu?s de 32,768 ms, lo que supera con creces el intervalo de muestra recomendado de 10 ms.
//No es necesario comprobar si hay desbordamiento. Si borra el contador en el flanco ascendente, entonces el conteo en el flanco descendente es el 
//ancho del pulso en microsegundos:
ISR(TIMER1_CAPT_vect) {

  switch(flanco){
        case 1:
            TCCR1B &=~ (1 << ICES1); //cambiamos el sensado establecido por el de descendente
            ancho_anterior = ancho;
            TCNT1 = 0x00;
            flanco++;
            break;
        default:
            
            ancho = 1000 - ((uint32_t) ICR1 * TIEMPO_MAXIMO_TIMER1_CAPTURE / REGISTRO_DE_COMPARACION_TIMER1);
            ancho_ok = (ancho <= ancho_anterior) ? ancho : ancho_ok;
            TCCR1B |= (1 << ICES1); //cambiamos el sensado establecido por el de ascendente
            flanco--;
            break;
  }

}
 ISR(ADC_vect)
 {    
     switch(opcion){
         case 0:
//                ema_a1 = (ALPHA*ADC)+((1-ALPHA)*ema_a1);
//                tension_red = (ema_a1 * 240)/1024; 
                ADMUX  = 0b01000000;
               opcion++;
                break;
         case 1:
                opcion--;
                break;
         case 2:
                ema_a0 = (ALPHA*ADC)+((1-ALPHA)*ema_a0);
                set_point = (ema_a0 * 25)/1024;   
                MAX7219_displayNumber(set_point);
//                ADMUX  = 0b01000001;
                opcion++;
                break;
          case 3:
//                ema_a1 = (ALPHA*ADC)+((1-ALPHA)*ema_a1);
//                tension_red = (ema_a1 * 240)/1024; 
//                ADMUX  = 0b01000000;
                opcion--;
                break;
         default:
                opcion = 0;
                ADMUX  = 0b01000001;
                break;
     } 
 }
 ISR(USART_RX_vect){
    rx = UDR0;	
    //implementamos case ya que es mas veloz
    switch(rx){
        case '¡':
            contador_rx_isr = 1;
            set_point = 0;
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
            set_point = set_point * contador_rx_isr + (rx - '0');
            contador_rx_isr = contador_rx_isr*10;
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
        DDRD = (1<<7);
    sei();
//    err = EEPROM_read(96, &set_point);
    set_point = 7;
    tension_red = 26;
    ancho = 50;
       angulo = 90;
       ton = 500;
    while(1){
        switch(seg){
            case 20:
                    angulo = (180*ancho_ok)/1000;
                    feedback =  CalcularTensionRMS();
                    ReajustarTension();
                    MostrarNumero(feedback, 1);
                    MostrarNumero(tension_red, 2);
                    MostrarNumero(ancho_ok, 3);
                    MostrarNumero(set_point, 4);
                    MostrarNumero(angulo, 5);
                    MAX7219_displayNumber(feedback);
//                    err = EEPROM_update(96, set_point);
                    seg = 0;
                    break;
            default:
                    break;
        }
    }
}
void MostrarNumero(uint16_t numero, uint8_t digitos){
	uint16_t _unidades = numero%10;
	uint16_t _decenas = (numero/10)%10;
    uint16_t _centenas = (numero/100)%10;  
    char _tx[4] = {'*', _centenas + '0', _decenas + '0', _unidades + '0' };
    switch(digitos){
        case 1:
                _tx[0] = '$';     
                break;
        case 2:
                _tx[0] = '%';
                break;
        case 3:
                _tx[0] =  '&';
                break;
        case 4:
                _tx[0] = '@';
                break;
        default:
                break;
    }
    USART_SetArrayData(_tx, 4);   
}
uint16_t  CalcularTensionRMS(){
    float _tensionRMS = ((tension_red*RAIZ_PI*RAIZ_2*sqrt((angulo*PI/90)-sin((angulo*PI/90))))/(PI_2)) + 1.55;
    return _tensionRMS;
}
void ReajustarTension(){
      int error = set_point-feedback;
      if(error != 0 && tension_red > feedback + 1 ){
        ton  =  ton + error * 2;
      }else if (tension_red == feedback + 1 && tension_red > set_point){
          ton = ton - 20;
      }
}