
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
volatile uint8_t us = 0;
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
  //Si es flanco ascendente lo dejamos pasar
  if(flanco){
      //En el flanco de subido establecemos todos los parametros en 0, si mantenemos hasta superar los 50us
      //o lograr un valor de flag mayor a 0 y el contador de clicks cortos es menor a 3 (por lo tanto nos encontramos
      //en el menu local) le damos el valor de true a clickLargo. Caso contraio aumenta el contador para pasar de menu.
      if((us >= 50 || flag > 0) && contador < 3){
        clickLargo = true;
      }else{
        contador++;
      }
  }
  //Establecemos valores iniciales de variables que se utilizaran en la rutina del timer
  flag = 0;
  us = 0;
  //En la primera interrupcion la variable flanco es falso por lo tanto la ponemos en true
  //para que en la proxima interrupcion al detectar el flanco descendente entre el el if
  flanco = !flanco;
}

ISR(INT1_vect){
    //Dimmer empieza como TRUE y su valor se maneja desde el USART
    if(dimmer){
       //Ponemos en alto la salida PD4 durante los usegundos hallados con la logica programada
       PORTD = (1<<4);
       _delay_us(usegundos);
       //Apagamos PD4
       PORTD = (0<<4);
    }
}


ISR(TIMER0_OVF_vect) {
    //Cada vez que se ejecute esta interrupcion (establecida para que sea en un lapso de 0,01ms)
    //aumentamos el valor de us en 1, cuando llega a 100 tendremos 1ms, incrementamos la bandera
    //para luego utilizarla en la logica de click largo y click corto. Ademas llamamos a la funcion
    //reajustar tension cada medio milisegundo. y volvemos a poner el registro del timer en el valor inicial
    us++;
    if(us == 100){
        us = 0;
        flag++;
<<<<<<< HEAD
        if(flag == 60){
         flag = 0;
         ReajustarTension();
        }else if(reajustar){
         ReajustarTension();
        } 
=======
        ReajustarTension(); 
    }else if( us == 50 ){
        ReajustarTension(); 
>>>>>>> main
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
		TCNT1 = 0x00; //Reestablecemos el valor de registro del timer para que empiece desde 0.
	}
  else // Detectamos un flanco descendente
	{
		captura = ICR1; // record time at which falling edge occurs
<<<<<<< HEAD
        ancho = 1000 -((captura*tMax)/rComp);
		TCCR1B |= (1 << ICES1); //cambiamos el sensado establecido por el de ascendente
       // TCCR1B|=(0<<CS12)|(0<<CS11)|(0<<CS10);
=======
        ancho = tSemiCiclo -((captura*tMax)/rComp); //Calculamos el ancho del pulso para tenerlo siempre actualizado
		TCCR1B |= (1 << ICES1); //cambiamos el sensado establecido por el de ascendente
>>>>>>> main
	}
     capturar = !capturar; //Calcular empieza en true, cuando detecta un flanco ascendente se pone en falso para que
     //en la proxima interrupcion producida por un flanco descendente tome el valor del ancho del pulso en ms.
}



void main() {
    cli();
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
<<<<<<< HEAD
        ReajustarTension();
=======
        //Si contador aumento por INT0 entre 1 y 2, se ejecuta la funcion ModoLocal.
>>>>>>> main
        if(contador == 0 || contador >= 3){
            //Volvemos contador igual a 0 por si es mayor a 2 asi poder entrar nuevamente al menu local.
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
    //Mostramos en el max la tension
    MAX7219_displayNumberyMenu(tensionRed, 1);
    //Obtenemos con el usart los enteros almacenados atraves de un puntero de direccion.
    USART_GetIntData(&tensionRedValida); //https://es.acervolima.com/como-devolver-multiples-valores-de-una-funcion-en-c-o-c/
    //Si la tension de red que obtuvimos es menor a 250 que es el valor maximo que soportara la electronica y es mayor a 10 que es el valor minimo.
    //Almacenamos la tension red valida y mostramos tanto en el display como por pantalla los datos establecidos.
    //Por el mostramos un mensaje de alerta por pantalla
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
    //Mostramos en el max la tension
    MAX7219_displayNumberyMenu(tensionDeseada, 2);
    //Obtenemos con el usart los enteros almacenados atraves de un puntero de direccion.
    USART_GetIntData(&tensionDeseadaValida); //https://es.acervolima.com/como-devolver-multiples-valores-de-una-funcion-en-c-o-c/
<<<<<<< HEAD
    if(tensionDeseadaValida <= tensionRed  && tensionDeseadaValida >= 5){
=======
    //Si la tension de dimer que obtuvimos es menor a la tension de red que es el valor maximo que soportara la electronica y es mayor a 5 que es el valor minimo.
    //Almacenamos la tension valida y mostramos tanto en el display como por pantalla los datos establecidos.
    //Por el mostramos un mensaje de alerta por pantalla
    if(tensionDeseadaValida < tensionRed - 11 && tensionDeseadaValida >= 5){
>>>>>>> main
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
    //Mostramos por pantalla que vamos a ajustar la tension de red de manera local.
    char _info[] ={"Ajustar Tension de Red Local \n\r"};
    USART_SetArrayData(_info, sizeof _info);
    //Mantenemos el ciclo mientras contador sigua siendo 1, lo que significa que seguimos ajustando
    //la tension
    while(contador == 1){
        //Leemos el ADC
        tensionRed = ADC_GetData(0)*220.0f/1024.0f;
       //Mostramos en el display
        MAX7219_displayNumberyMenu(tensionRed, 1);
        
    }
    //Volvemos a contador = 1 para no salir del menu y que nos muestre la tension seteada en el display
    contador = 1;
    //Volvemos click largo a falso asi no entra a la funcion nuevamente
    clickLargo = false;
    //Mostramos un mensaje que se establecio con exito
    char _confirmacion[] ={"Se establecio una Tension de red de: \n\r"};
    USART_SetArrayData(_confirmacion, sizeof _confirmacion);
    //Imprimimos en pantalla el valor establecido
    MostrarNumero(tensionRed, 3);
}

void AjustarTensionDimmerLocal(){
    //Mostramos por pantalla que vamos a ajustar la tension del dimmer de manera local.
    char _info[] ={"Ajustar Tension de Dimmer Local \n\r"};
    USART_SetArrayData(_info, sizeof _info);
    //Mantenemos el ciclo mientras contador sigua siendo 2, lo que significa que seguimos ajustando
    while(contador == 2){  
        //Leemos el ADC, el valor resultante ira de 0 a 100 que luego multiplicara a la tension de red
        //para no pasar su valor
        porciento = (ADC_GetData(0)*100.0f)/1024.0f;
        tensionDeseada = (porciento * tensionRed)/100;
        //Mostramos en el display
        MAX7219_displayNumberyMenu(tensionDeseada, 2);
<<<<<<< HEAD
        y = (tensionDeseada*10);
        ReajustarTension();
=======
>>>>>>> main
    }
    //Volvemos a contador = 2 para no salir del menu y que nos muestre la tension seteada en el display
    contador = 2;
    //Volvemos click largo a falso asi no entra a la funcion nuevamente
    clickLargo = false;
    //Mostramos un mensaje que se establecio con exito
    char _confirmacion[] ={"Se establecio una Tension en la carga de: \n\r"};
    USART_SetArrayData(_confirmacion, sizeof _confirmacion);
    //Imprimimos en pantalla el valor establecido
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
<<<<<<< HEAD
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
=======
    //Regla de 3 simples para hallar el angulo con los ms obtenidos por el timer
    angulo = (180*ancho)/100;
    //Formula para hallar la tension RMS en la carga
    uint16_t _tensionRMSCarga =  sqrt((pow(tensionRed , 2)*(angulo*PI/180))/(2*PI)-(pow(tensionRed , 2)*(sin(2*(angulo * PI/180)))/(4*PI)))*100;
    return _tensionRMSCarga;
}

uint16_t  CalcularTensionDC(){
    //Regla de 3 simples para hallar el angulo con los ms obtenidos por el timer
    angulo = (180*ancho)/100;
    //Formula para hallar la tension DC en la carga
    uint16_t _tensionDCCarga =   (tensionRed*sqrt(2)/PI)*(1-cos(angulo * PI/180))*100;
    return _tensionDCCarga;
}


uint16_t CalcularRetardo(uint16_t tension){
    //Calculamos el angulo con la tension media que le mandemos
    uint16_t _angulo = acos(tension/(tensionRed*sqrt(2)/PI)-1)* 180/PI;
    //Con regla de 3 simples obtenemos los micro segundos
    uint16_t _usegundos = 1000 - ((100*_angulo)/180)* 100;
    return _usegundos ;
}

void ReajustarTension(){
    //Obtenemos los valores iniciales
    uint16_t x = CalcularTensionRMS();
    uint16_t y = (tensionDeseada*100);
    uint16_t diff = (y - x);
    //Para que no oscile bruscamente y tenga un reajuste suave. Limitamos el reajuste
    //a 3v cada medio milisegundo.
    if(diff > 500){
        diff = 300;
    }
    //Si el valor RMS se encuentra entre los +-1,5V del valor deseado las instrucciones
    //del if no se ejecutan.
    //Si la diferencia es mayor, se busca saber si la tension RMS obtenida es mayor o menor
    //Si es mayor se le resta la diferencia a la variable de ajuste, si es menor se le suma.
    if(x>=y+150 || x<=y-150){
        if(x < y){       
          tensionReajustada = tensionReajustada + (diff );
          usegundos = CalcularRetardo((tensionReajustada/100));
        }else if(x > y){
          tensionReajustada = tensionReajustada - (diff );
          usegundos = CalcularRetardo((tensionReajustada/100));
>>>>>>> main
        }
        
    }else{
        reajustar = false;
    }
}