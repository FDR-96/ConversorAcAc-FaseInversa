
#ifndef MAX7219_H_
#define MAX7219_H_



#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <xc.h>



// Outputs, pin definitions
#define PIN_SCK                   PORTB5
#define PIN_MOSI                  PORTB3
#define PIN_SS                    PORTB2



#define MAX7219_LOAD1             PORTB |= (1<<PIN_SS)  
#define MAX7219_LOAD0             PORTB &= ~(1<<PIN_SS)

#define MAX7219_MODE_DECODE       0x09
#define MAX7219_MODE_INTENSITY    0x0A
#define MAX7219_MODE_SCAN_LIMIT   0x0B
#define MAX7219_MODE_POWER        0x0C
#define MAX7219_MODE_TEST         0x0F
#define MAX7219_MODE_NOOP         0x00

// I only have 3 digits, no point having the
// rest. You could use more though.
#define MAX7219_DIGIT0            0x01U
#define MAX7219_DIGIT1            0x02U
#define MAX7219_DIGIT2            0x03U
#define MAX7219_DIGIT3            0x04U
#define MAX7219_DIGIT4            0x05U
#define MAX7219_DIGIT5            0x06U
#define MAX7219_DIGIT6            0x07U
#define MAX7219_DIGIT7            0x08U


#define MAX7219_CHAR_BLANK        0xF 
#define MAX7219_CHAR_NEGATIVE     0xA 

#define		ON			0x01U       //Definimos 
#define		OFF			0x00U


#define DISP_SELECT() SPI_PORT &= ~(1<<SPI_DISP)
#define DISP_DESELECT() SPI_PORT |= (1<<SPI_DISP)




//uint8_t dig;
//uint8_t	inf;


//static const struct {
//	char   ascii;
//	char   segs;
//} MAX7219_Font[] = {
//  {'A',0b1110111},{'B',0b1111111},{'C',0b1001110},{'D',0b1111110},{'E',0b1001111},{'F',0b1000111},       
//  {'G',0b1011110},{'H',0b0110111},{'I',0b0110000},{'J',0b0111100},{'L',0b0001110},{'N',0b1110110},       
//  {'O',0b1111110},{'P',0b1100111},{'R',0b0000101},{'S',0b1011011},{'T',0b0001111},{'U',0b0111110},       
//  {'Y',0b0100111},{'[',0b1001110},{']',0b1111000},{'_',0b0001000},{'a',0b1110111},{'b',0b0011111},       
//  {'c',0b0001101},{'d',0b0111101},{'e',0b1001111},{'f',0b1000111},{'g',0b1011110},{'h',0b0010111},       
//  {'i',0b0010000},{'j',0b0111100},{'l',0b0001110},{'n',0b0010101},{'o',0b1111110},{'p',0b1100111},       
//  {'r',0b0000101},{'s',0b1011011},{'t',0b0001111},{'u',0b0011100},{'y',0b0100111},{'-',0b0000001},
//  {' ',0b0000000},{'0',0b1111110},{'1',0b0110000},{'2',0b1101101},{'3',0b1111001},{'4',0b0110011},
//  {'5',0b1011011},{'6',0b1011111},{'7',0b1110000},{'8',0b1111111},{'9',0b1111011},{'\0',0b0000000},
//  };

//
////Definicion de funciones
//void Display_Test(uint8_t inf);
//void Display_Intensity(uint8_t inf);
//void Display_Scan(uint8_t inf);
//void Display_Shutdown(uint8_t inf);
//void Display_Decode(uint8_t inf);
//void Display_Digit(uint8_t dig, uint8_t inf);
//void Display_Clear();

void MAX7219_init ();
void spiSendByte (char databyte);
void MAX7219_writeData(char data_register, char data);
void MAX7219_clearDisplay();
void MAX7219_displayNumber(volatile long number);

//void MAX7219_Display_Text(char *text, int justify);



#endif /* MAX7219_H_ */