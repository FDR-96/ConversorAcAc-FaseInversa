
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
volatile uint16_t variacionTensionRed = 0;
volatile uint16_t setPoint = 10;

volatile uint16_t tensionDeseadaValida = 0;
volatile uint16_t tensionReajustada = 50;
volatile uint16_t captura = 0; //Capturara el valor del timer
volatile uint32_t ancho = 0; //Valor final del ancho de pulso
volatile uint16_t angulo = 0;
volatile uint16_t usegundos = 500;
volatile uint16_t porciento = 0;
volatile uint16_t ton = 0;
volatile uint8_t flag = 0;
volatile uint8_t contador = 0;

volatile uint16_t ema =  0;
volatile uint16_t ema1 =  0;
float alpha = 0.05;
volatile uint16_t feedback = 0;    

volatile uint8_t err = 0;
uint8_t byte1 = 0;
uint8_t byte2 = 0;

volatile unsigned long usCentena = 0; //0-10
 volatile uint32_t ms = 0; //0-1000
volatile uint16_t msSemiCiclo = 0; //0-10
volatile uint16_t semiCiclo = 0; //0-1000
volatile uint16_t seg = 0; //0-100
volatile uint16_t lastSeg = 0;

uint8_t RMS;
int8_t rangoErr, errorAnterior, divErr;  
unsigned char rx = 'o';
unsigned char tx = '+';   
uint16_t y = 50;
const double PI = 3.14159265358979323846;

volatile bool dimmer = true;
volatile bool click = false;
volatile bool bandera = true;
volatile bool capturar = true;
volatile bool clickLargo = false;
volatile bool reajustar = true;
volatile bool almacenar = false;
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
     click = !click;
}

ISR(INT1_vect){
    if(dimmer){
        TCNT0 = 0xec;
        PORTD = (1<<7);
        msSemiCiclo = 0;
        seg++;
    }
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
     if(cambio && !click){
        ema = (0.03*ADC)+((1-0.03)*ema);
        tensionRed = (ema * 240)/1024; 
     }else if(click && !cambio){
       ema1 = (alpha*ADC)+((1-alpha)*ema1);
       porciento = ((ema1 * 100)/1024);
       setPoint = tensionRed*porciento/100;  
       MAX7219_displayNumber(setPoint);
       cli();
           ADMUX |=  (0<<MUX0);
       sei();
     }else if(click && cambio){
        ema = (alpha*ADC)+((1-alpha)*ema);
        tensionRed = (ema * 240)/1024; 
        cli();
            ADMUX |=  (1<<MUX0);
        sei();
     }
     cambio = !cambio;
 }
 
 ISR(USART_RX_vect){
    rx = UDR0;	
    
    if(rx == 'o'){
        tx = '+'; 
        USART_SetData('!');
        dimmer = true;
    }else if(rx == 'x'){
        tx = '-'; 
        USART_SetData('!');
        dimmer = false;
    }else if(rx == '!'){
        USART_SetData('!');
 //       setPoint = USART_GetIntData();
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
//    err = EEPROM_read(96, &setPoint);
//    err = EEPROM_read(102, &byte1);
//    err = EEPROM_read(103, &byte2);
//    usegundos =  (byte1 * 100 + byte2);
//    if(err != 0 || usegundos >= 100){
//        usegundos =  5000;
//    }
//lastSeg = seg;
//    Interfaz(1);
    
    while(1){
//        if(err == 0 && almacenar){
//            byte1 = usegundos/100;
//            byte2 = usegundos - byte1 * 100;
//            EEPROM_update(102, byte1);
//            EEPROM_update(103, byte2);
//        }
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
//            rx = USART_GetData();
//                if(rx == 'o'){
//                    tx = '+'; 
//                    dimmer = true;
//                }else if(rx == 'x'){
//                    tx = '-'; 
//                    dimmer = false;
//                }else if(rx == '!'){
//                    USART_SetData('!');
//                    setPoint = USART_GetIntData();
//           //         AjustarTensionDimmerRemoto();
//                }
//            
            seg = 0;
        }
        
     //    ton = usegundos/2 + usegundos/5;

//        if(contador == 0 || contador >= 2){
//            contador = 0;
//            //ModoRemoto();
//        }else{
//            ModoLocal();
//        }
    }
}


void ModoRemoto(){
    switch(USART_GetData()){
        case '1':
            AjustarTensionDimmerRemoto();
            break;
        case '2':
            dimmer = !dimmer;
            if(dimmer){
           //   TCCR0B |= (1<<CS00)|(0<<CS01)|(0<<CS02);	
              char _mensajeTrue[] ={"\n\r Dimmer encendido \n\r"};
              USART_SetArrayData(_mensajeTrue, sizeof _mensajeTrue);
             // TIMSK0 |= (1<<TOIE0);
            }else{
            //  TCCR0B |= (0<<CS00)|(0<<CS01)|(0<<CS02);
              char _mensajeFalse[] ={"\n\r Dimmer Apagado\n\r"};
              USART_SetArrayData(_mensajeFalse, sizeof _mensajeFalse);
              //TIMSK0 &=~ (1<<TOIE0);
            }
            break;
        case '3':
            Interfaz(0);
            break;
        case '4':
            Interfaz(1);
            break;
        default:
            return;

    }
}

//void Interfaz(int8_t opciones){
//    if(opciones){
//      char _linea1[] ={"\n\rSeleccione la accion que desee realizar: \n\r"};
//      USART_SetArrayData(_linea1, sizeof _linea1);
//      char _linea2[] ={" 1. Modificar SetPoint. \n\r"};
//      USART_SetArrayData(_linea2, sizeof _linea2);
//      char _linea3[] ={" 2. Encender/Apagar convertidor AC/AC. \n\r"};
//      USART_SetArrayData(_linea3, sizeof _linea3);      
//      char _linea4[] ={" 3. Mostrar valores. \n\r"};
//      USART_SetArrayData(_linea4, sizeof _linea4);
//      char _linea5[] ={" 4. Mostrar menu. \n\r"};
//      USART_SetArrayData(_linea5, sizeof _linea5);
//      char _linea6[] ={"\n\r |   Tension de Red   | Tension de Seteada | Tension RMS |   Ton   | Angulo |"};
//      USART_SetArrayData(_linea6, sizeof _linea6);
//    }
//
//      char _1[] ={"\n\r |        "};
//      char _2[] ={"V        |       "};
//      char _2b[] ={"V         |    "};
//      USART_SetArrayData(_1, sizeof _1);
//      MostrarNumero(tensionRed, 3);
//      USART_SetArrayData(_2, sizeof _2);
//      MostrarNumero(setPoint, 3);
//      USART_SetArrayData(_2b, sizeof _2b);
//      MostrarNumero(CalcularTensionRMS(), 3);
//      char _3[] ={"V     |  "};
//      char _4[] ={" |  "};
//      USART_SetArrayData(_3, sizeof _3);
//      MostrarNumero(ancho/10, 6);
//      USART_SetArrayData(_4, sizeof _4);
//        char _5[] ={"\217  |"};
//      MostrarNumero(angulo, 3);
//      USART_SetArrayData(_5, sizeof _5);
//
//      
//}
void AjustarTensionDimmerRemoto(){
   // char _info[] ={"\n\r \n\rAjustar Tension de Dimmer Local: "};
  //  USART_SetArrayData(_info, sizeof _info);
  //  MAX7219_displayNumberyMenu(setPoint, 2);
    tensionDeseadaValida = USART_GetIntData(); //https://es.acervolima.com/como-devolver-multiples-valores-de-una-funcion-en-c-o-c/
    if(tensionDeseadaValida <= tensionRed  && tensionDeseadaValida >= 5){
        setPoint = tensionDeseadaValida ;
    //    char _confirmacion[] ={"\n\r Se establecio el valor ingresado: "};
    //    USART_SetArrayData(_confirmacion, sizeof _confirmacion);
        MostrarNumero(setPoint, 4);
    //    err = EEPROM_update(96, setPoint);
   //     MAX7219_displayNumberyMenu(setPoint, 2);
   
    }else{
     //   char _alerta[] ={"\n\r Alerta!, ingrese otro valor. \n\r"};
     //   USART_SetArrayData(_alerta, sizeof _alerta);
    }
  
}

void ModoLocal(){
        switch(contador){
        case 1:
             MAX7219_displayNumber(setPoint);
            if(clickLargo){AjustarTensionDimmerLocal();}
            break;
        case 2:
            MAX7219_displayNumber(setPoint);
            if(clickLargo){AjustarTensionDimmerLocal();}
            break;
        default:
            return;
    }
}

void AjustarTensionDimmerLocal(){
    char _info[] ={"\n\r Ajustar Tension de Dimmer Local \n\r"};
    USART_SetArrayData(_info, sizeof _info);
    while(contador == 2){  
        porciento = (ADC_GetData(0)*100.0f)/1024.0f;
        setPoint = (porciento * tensionRed)/100;
        MAX7219_displayNumberyMenu(setPoint, 2);
    }
    err = EEPROM_update(96, setPoint);
    contador = 2;
    clickLargo = false;
    char _confirmacion[] ={"\n\r Se establecio una Tension en la carga de: "};
    USART_SetArrayData(_confirmacion, sizeof _confirmacion);
    MostrarNumero(setPoint, 2);
}

void MostrarNumero(uint16_t numero, uint8_t digitos)
{
	uint16_t _unidades = numero%10;
	uint16_t _decenas = (numero/10)%10;
    uint16_t _centenas = (numero/100)%10;
//    uint16_t _milesu = (numero/1000)%10;
//    uint16_t _milesd = (numero/10000)%10;


    
    
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
       
      if(fabs(error)!= 0 && tensionRed > feedback + 1){
        int diff = 1;
        if(error > 1){
            diff = -1;
        }
        ton  =  ton + diff + error * 2;
      }
  }