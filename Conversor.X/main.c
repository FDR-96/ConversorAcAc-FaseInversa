
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
//#include "Librerias/PCINT/PCINT.h"
#include "Librerias/TIMER1/TIMER1.h"
#include "Librerias/ADC/ADC.h"
#include "Librerias/USART/USARTAtmega328P.h"
#include "Librerias/MAX7219/Max7219.h"


//Declaracion de variables.
const uint32_t tMax = 3276; //Tiempo maximo que puede alcanzar el timer sin desbordar 
const uint32_t rComp = 65536; //Registro de comparacion maximo de 8bit
const uint16_t tSemiCiclo = 100; //Tiempo de un semiciclo
volatile uint16_t tensionRed = 110;
volatile uint16_t tensionRedValida = 0;
volatile uint16_t tensionDeseada = 50;
volatile uint16_t tensionDeseadaValida = 0;
volatile uint16_t tensionReajustada = 50;
volatile uint16_t captura = 0; //Capturara el valor del timer
volatile uint32_t ancho = 0; //Valor final del ancho de pulso
volatile uint16_t angulo = 0;
volatile uint16_t usegundos = 0;
volatile uint16_t porciento = 0;
volatile uint8_t msegundos = 0;
volatile uint8_t flag = 0;
volatile uint8_t contador = 0;

uint16_t y = 500;
const double PI = 3.14159265358979323846;


volatile bool dimmer = true;
volatile bool flanco = false;
volatile bool bandera = true;
volatile bool capturar = true;
volatile bool clickLargo = false;
volatile bool reajustar = true;
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
uint16_t CalcularTensionRMS();

//Rutinas de servicio de interrupción.
ISR(INT0_vect){
  if(flanco){
      if((msegundos >= 50 || flag > 0) && contador < 3){
        clickLargo = true;
      }else{
        contador++;
      }
  }
  flag = 0;
  msegundos = 0;
  flanco = !flanco;
}

ISR(INT1_vect){
    if(dimmer){
       PORTD = (1<<4);
       _delay_us(usegundos);
       PORTD = (0<<4);
    }
}


ISR(TIMER0_OVF_vect) {
    msegundos++;
    if(msegundos == 100){
        msegundos = 0;
        flag++;
        if(flag == 60){
         flag = 0;
         ReajustarTension();
        }else if(reajustar){
         ReajustarTension();
        } 
    }
    TCNT0 = 0x63;
}

//Si el TC (de 16 bits) se ejecuta a 16 MHz, se desbordará después de 32,768 ms, lo que supera con creces el intervalo de muestra recomendado de 10 ms.
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
        ancho = 1000 -((captura*tMax)/rComp);
		TCCR1B |= (1 << ICES1); //cambiamos el sensado establecido por el de ascendente
       // TCCR1B|=(0<<CS12)|(0<<CS11)|(0<<CS10);
	}
     capturar = !capturar;
}



void main() {
    cli();
       // PCINT_init();
        INT_init();
        TIMER0_init();
        TIMER1_init();
        ADC_init();
        USART_init();
        MAX7219_init();
        DDRD = (1<<4);
        PORTD = (0<<4);
    sei();
    Interfaz();
    while(1){
        ReajustarTension();
        if(contador == 0 || contador >= 3){
            contador = 0;
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
          //  MostrarNumero(tensionReajustada, 5);
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
      char _linea4[] ={" |   Tension de Red   | Tension de Seteada | Tension RMS |   Ton   | Angulo | \n\r"};
      USART_SetArrayData(_linea4, sizeof _linea4);
   
      char _1[] ={" |        "};
      char _2[] ={"V        |       "};
      char _2b[] ={"V         |   "};
      USART_SetArrayData(_1, sizeof _1);
      MostrarNumero(tensionRed, 3);
      USART_SetArrayData(_2, sizeof _2);
      MostrarNumero(tensionDeseada, 3);
      USART_SetArrayData(_2b, sizeof _2b);
      MostrarNumero(CalcularTensionRMS(), 5);
      char _3[] ={"V   |  "};
      char _4[] ={" |  "};
      USART_SetArrayData(_3, sizeof _3);
      MostrarNumero(usegundos/100, 6);
      USART_SetArrayData(_4, sizeof _4);
        char _5[] ={"\217  |"};
      MostrarNumero(angulo, 3);
      USART_SetArrayData(_5, sizeof _5);

      
}

void AjustaTensionDeRedRemoto(){
    MAX7219_displayNumberyMenu(tensionRed, 1);
    USART_GetIntData(&tensionRedValida); //https://es.acervolima.com/como-devolver-multiples-valores-de-una-funcion-en-c-o-c/
    if(tensionRedValida < 250 && tensionRedValida >= 10){
        tensionRed = tensionRedValida;
        char _confirmacion[] ={"Se establecio el valor ingresado: \n\r"};
        USART_SetArrayData(_confirmacion, sizeof _confirmacion);
        MostrarNumero(tensionRed, 3);
        MAX7219_displayNumberyMenu(tensionRed, 1);
    }else{
        char _alerta[] ={"Alerta!, ingrese otro valor. \n\r"};
        USART_SetArrayData(_alerta, sizeof _alerta);
    }
}

void AjustarTensionDimmerRemoto(){
    MAX7219_displayNumberyMenu(tensionDeseada, 2);
    USART_GetIntData(&tensionDeseadaValida); //https://es.acervolima.com/como-devolver-multiples-valores-de-una-funcion-en-c-o-c/
    if(tensionDeseadaValida <= tensionRed  && tensionDeseadaValida >= 5){
        tensionDeseada = tensionDeseadaValida;
        char _confirmacion[] ={"Se establecio el valor ingresado: \n\r"};
        USART_SetArrayData(_confirmacion, sizeof _confirmacion);
        MostrarNumero(tensionDeseada, 3);
        y = (tensionDeseada*10);
        MAX7219_displayNumberyMenu(tensionDeseada, 2);
        ReajustarTension();
    }else{
        char _alerta[] ={"Alerta!, ingrese otro valor. \n\r"};
        USART_SetArrayData(_alerta, sizeof _alerta);
    }
  
}

void ModoLocal(){
        switch(contador){
        case 1:
            MAX7219_displayNumberyMenu(tensionRed, 1);
            if(clickLargo){AjustaTensionDeRedLocal();}
            break;
        case 2:
            MAX7219_displayNumberyMenu(tensionDeseada, 2);
            if(clickLargo){AjustarTensionDimmerLocal();}
            break;
        default:
            return;
    }
}

void AjustaTensionDeRedLocal(){
    char _info[] ={"Ajustar Tension de Red Local \n\r"};
    USART_SetArrayData(_info, sizeof _info);
    while(contador == 1){
        tensionRed = ADC_GetData(0)*220.0f/1024.0f;
       // tensionDeseada = (porciento * tensionRed)/100;
        MAX7219_displayNumberyMenu(tensionRed, 1);
        
    }
    contador = 1;
    clickLargo = false;
    char _confirmacion[] ={"Se establecio una Tension de red de: \n\r"};
    USART_SetArrayData(_confirmacion, sizeof _confirmacion);
    MostrarNumero(tensionRed, 3);
}

void AjustarTensionDimmerLocal(){
    char _info[] ={"Ajustar Tension de Dimmer Local \n\r"};
    USART_SetArrayData(_info, sizeof _info);
    while(contador == 2){  
        porciento = (ADC_GetData(0)*100.0f)/1024.0f;
        tensionDeseada = (porciento * tensionRed)/100;
        MAX7219_displayNumberyMenu(tensionDeseada, 2);
        y = (tensionDeseada*10);
        ReajustarTension();
    }
    contador = 2;
    clickLargo = false;
    char _confirmacion[] ={"Se establecio una Tension en la carga de: \n\r"};
    USART_SetArrayData(_confirmacion, sizeof _confirmacion);
    MostrarNumero(tensionDeseada, 3);
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
        char _tx4[5] ={ _milesu + '0', _centenas + '0','x',_decenas + '0', _unidades + '0' };
        USART_SetArrayData(_tx4, sizeof _tx4);
    }else if(digitos == 5){
        char _tx5[6] ={_milesd + '0', _milesu + '0', _centenas + '0',',',_decenas + '0', _unidades + '0'};
        USART_SetArrayData(_tx5, sizeof _tx5);
    }else{
        char _tx2[6] ={_centenas + '0',_decenas + '0',',', _unidades + '0','m','s'};
        USART_SetArrayData(_tx2, sizeof _tx2);
    }

}

uint16_t  CalcularTensionRMS(){
    angulo = (180*ancho)/1000;

    uint16_t _tensionRMSCarga1 =  ((tensionRed*sqrt(2))*sqrt((angulo*PI/180)-(sin(2*((angulo*PI/180)+PI))/2))/(2*sqrt(PI)))*100;
    uint16_t _tensionRMSCarga2 =  ((tensionRed*sqrt(2))*sqrt((angulo*PI/180)-(sin(2*((angulo*PI/180)))/2))/(2*sqrt(PI)))*100;
    uint16_t _tensionRMSCarga = sqrt(pow(_tensionRMSCarga1, 2)+pow(_tensionRMSCarga2, 2));
    return _tensionRMSCarga;
}

void ReajustarTension(){
    uint16_t x = CalcularTensionRMS()/10;
    if(x>=y+5 || x<=y-5){
        reajustar = true;
        
        
        if(x>=y+10 || x<=y-10){
            usegundos = usegundos + ((y - x)*5);
        }else if(x>=y+20 || x<=y-20){
            usegundos = usegundos + ((y - x)*1000);
        }else if(x>=y+700 || x<=y-700){
            usegundos = usegundos + ((y-x)/10);
        }else if(x>=y+1000 || x<=y-1000){
            usegundos = usegundos + ((y-x)/1000);
        }
        
    }else{
        reajustar = false;
    }
}