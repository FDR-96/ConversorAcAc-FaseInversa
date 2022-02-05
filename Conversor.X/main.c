
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
#include "Librerias/ADC/ADC.h"
#include "Librerias/USART/USARTAtmega328P.h"
#include "Librerias/MAX7219/Max7219.h"


//Declaracion de variables.
const uint16_t tMax = 163; //Tiempo maximo que puede alcanzar el timer sin desbordar 
const uint16_t rComp = 256; //Registro de comparacion maximo de 8bit
const uint16_t tSemiCiclo = 100; //Tiempo de un semiciclo
volatile uint16_t tensionRed = 110;
volatile uint16_t tensionRedValida = 0;
volatile uint16_t tensionDeseada = 50;
volatile uint16_t tensionDeseadaValida = 0;
volatile uint16_t tensionReajustada = 0;
volatile uint16_t captura = 0; //Capturara el valor del timer
volatile uint16_t ancho = 0; //Valor final del ancho de pulso
volatile uint16_t angulo = 0;
volatile uint16_t usegundos = 0;
volatile uint16_t msegundos = 0;
volatile uint8_t segundos = 0;
volatile uint8_t flag = 0;
volatile uint8_t contador = 0;

const double PI = 3.14159265358979323846;


volatile bool dimmer = true;
volatile bool flanco = false;
volatile bool bandera = true;
volatile bool capturar = true;
volatile bool clickLargo = false;
volatile bool reajustar = false;
//bool buffer = false;


//Declaracion de Funciones.
//Funciones exclusivas del Modo Remoto.
void ModoRemoto();
void Interfaz();
void AjustaTensionDeRedRemoto();
void AjustarTensionDimmerRemoto();
//Funciones exclusivas del Modo Local.
void ModoLocal();
void AjustaTensionDeRedLocal();
void AjustarTensionDimmerLocal();
//Funciones generales utilizadas por ambos modos.
void MostrarNumero(uint16_t numero, uint8_t digitos);  //Digitos = 3: xxx / Digitos = 4: xx,xx / Digitos = 5: xxx,xx
void ReajustarTension();
uint16_t CalcularTension();
uint16_t CalcularRetardo(uint16_t tension);


//Rutinas de servicio de interrupción.
ISR(INT0_vect){
    if(dimmer){
       PORTD = (1<<4);
       _delay_us(usegundos);
       PORTD = (0<<4);
    }
}

ISR(INT1_vect){
  if(flanco){
      if(msegundos >= 50 || flag > 0){
        clickLargo = true;
      }else if(contador >= 3){
        contador = 0;
        bandera = true;
      }else{
        bandera = false;
        contador++;
      }
  }
  flag = 0;
  msegundos = 0;
  flanco = !flanco;
}

ISR(TIMER0_OVF_vect) {
    msegundos++;
    if(msegundos == 100){
        msegundos = 0;
        flag++;
    }else if(msegundos == 20 || msegundos == 50 || msegundos == 80 ){
            TCCR1B|=(1<<CS12)|(0<<CS11)|(1<<CS10);
            reajustar = true;
    }
    TCNT0 = 0x63;
}

//Si el TC (de 8 bits) se ejecuta a 16 MHz, se desbordará después de 16,384 ms, lo que supera con creces el intervalo de muestra recomendado de 10 ms.
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
        ancho = tSemiCiclo -((captura*tMax)/rComp);
		TCCR1B |= (1 << ICES1); //cambiamos el sensado establecido por el de ascendente
        TCCR1B|=(0<<CS12)|(0<<CS11)|(0<<CS10);
	}
     capturar = !capturar;
}



void main() {
    cli();
        PCINT_init();
        INT_init();
        TIMER0_init();
        TIMER1_init();
        ADC_init();
        USART_init();
        MAX7219_init();
        DDRD = (1<<4);
        PORTD = (0<<4);
    sei();
    usegundos = CalcularRetardo(tensionDeseada);
    Interfaz();
//         MostrarNumero(123, 3);
//         USART_SetData('d');
//        MostrarNumero(2232, 4);
//                 USART_SetData('x');
//        MostrarNumero(33332, 5);
    while(1){
//   MostrarNumero(CalcularTension(), 5);
//   USART_SetData('s');
        ReajustarTension();  
        if(bandera){
            ModoRemoto();
        }else{
            ModoLocal();
        }
    }
}


void ModoRemoto(){
    switch(USART_GetData()){
        case '1':
            AjustaTensionDeRedRemoto();
            break;
        case '2':
            AjustarTensionDimmerRemoto();
            break;
        case '3':
            dimmer = false;
            break;
        case '4':
            dimmer = true;
            break;
        case 'x':
            Interfaz();
            break;
        default:
            return;

    }
}

void Interfaz(){
      char _linea1[] ={"Seleccione la accion que desee realizar: \n\r"};
      USART_SetArrayData(_linea1, sizeof _linea1);
      char _linea2[] ={" 1. Modificar la Tension de Red. \n\r"};
      USART_SetArrayData(_linea2, sizeof _linea2);
      char _linea3[] ={" 2. Modificar la Tension del Dimmer. \n\r"};
      USART_SetArrayData(_linea3, sizeof _linea3);
      char _linea4[] ={" |   Tension de Red   | Tension de Dimmer |Tension en la Carga|  Ton   | Angulo | \n\r"};
      USART_SetArrayData(_linea4, sizeof _linea4);
   
      char _1[] ={" |        "};
      char _2[] ={"V        |       "};
      USART_SetArrayData(_1, sizeof _1);
      MostrarNumero(tensionRed, 3);
      USART_SetArrayData(_2, sizeof _2);
      MostrarNumero(tensionDeseada, 3);
      USART_SetArrayData(_2, sizeof _2);
      MostrarNumero(CalcularTension(), 5);
      char _3[] ={"V      | "};
      char _4[] ={" |  "};
      USART_SetArrayData(_3, sizeof _3);
      MostrarNumero(ancho, 6);
      USART_SetArrayData(_4, sizeof _4);
        char _5[] ={"\217 |"};
      MostrarNumero(angulo, 3);
      USART_SetArrayData(_5, sizeof _5);
}

void AjustaTensionDeRedRemoto(){
    MAX7219_displayNumberyMenu(tensionRed, 1);
    USART_GetIntData(&tensionRedValida); //https://es.acervolima.com/como-devolver-multiples-valores-de-una-funcion-en-c-o-c/
    if(tensionRedValida < 250 && tensionRedValida >= 10){
        tensionRed = tensionRedValida;
        char _confirmacion[] ={"Se establecio el valor ingresado: \n"};
        USART_SetArrayData(_confirmacion, sizeof _confirmacion);
        MostrarNumero(tensionRed, 3);
        MAX7219_displayNumberyMenu(tensionRed, 1);
        usegundos = CalcularRetardo(tensionDeseada);
    }else{
        char _alerta[] ={"Alerta!, ingrese otro valor. \n"};
        USART_SetArrayData(_alerta, sizeof _alerta);
    }
}

void AjustarTensionDimmerRemoto(){
    MAX7219_displayNumberyMenu(tensionDeseada, 2);
    USART_GetIntData(&tensionDeseadaValida); //https://es.acervolima.com/como-devolver-multiples-valores-de-una-funcion-en-c-o-c/
    if(tensionDeseadaValida < tensionRed - 11 && tensionDeseadaValida >= 5){
        tensionDeseada = tensionDeseadaValida;
        char _confirmacion[] ={"Se establecio el valor ingresado: \n"};
        USART_SetArrayData(_confirmacion, sizeof _confirmacion);
        MostrarNumero(tensionDeseada, 3);
        MAX7219_displayNumberyMenu(tensionDeseada, 2);
        usegundos = CalcularRetardo(tensionDeseada);
    }else{
        char _alerta[] ={"Alerta!, ingrese otro valor. \n"};
        USART_SetArrayData(_alerta, sizeof _alerta);
    }
}

void ModoLocal(){
        switch(contador){
        case 1:
            MAX7219_displayNumberyMenu(tensionRed, 1);
            if(clickLargo)AjustaTensionDeRedLocal();
            break;
        case 2:
            MAX7219_displayNumberyMenu(tensionDeseada, 2);
            if(clickLargo){
                   MAX7219_displayNumberyMenu(222, 2);
                   clickLargo = false;
            }
            break;
        default:
            return;
    }
}

void AjustaTensionDeRedLocal(){
    while(contador == 1){
        tensionRed = ADC_GetData(0)*220.0f/1024.0f;
        MAX7219_displayNumberyMenu(tensionRed, 1);
    }
    clickLargo = false;
}

void AjustarTensionDimmerLocal(){
        clickLargo = false;
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
    uint16_t _tensionCarga =  (tensionRed*sqrt(2)/PI)*(1-cos(angulo * PI/180));
    return _tensionCarga * 100;
}

uint16_t CalcularRetardo(uint16_t tension){
    uint16_t _angulo = acos(tension/(tensionRed*sqrt(2)/PI)-1)* 180/PI;
    uint16_t _usegundos = (100*_angulo)/180;
    return 10000 - _usegundos * 100 ;
}

void ReajustarTension(){
    tensionReajustada = CalcularTension();
 
    while(tensionReajustada <= tensionDeseada ){
    tensionReajustada++;
    usegundos = CalcularRetardo(tensionReajustada);
    }
    while(tensionReajustada >= tensionDeseada){
    tensionReajustada--;
    usegundos = CalcularRetardo(tensionReajustada);
    }
 
}