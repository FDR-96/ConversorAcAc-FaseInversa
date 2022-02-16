/*
 * File:   Max7219.c
 * Author: FDPR
 *
 * Created on 4 de abril de 2021, 14:12
 */


#include "MAX7219.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define SEGA        0b01000000
#define SEGB        0b00100000
#define SEGC        0b00010000
#define SEGD        0b00001000
#define SEGE        0b00000100
#define SEGF        0b00000010
#define SEGG        0b00000001
#define SEGDOT		0b10000000

uint8_t chars={
    SEGF|SEGE|SEGD|SEGC|SEGB           // v (looks like a U)
    };
char digitsInUse = 8;

void MAX7219_init(){
    // SCK MOSI CS/LOAD/SS
    DDRB |= (1 << PIN_SCK) | (1 << PIN_MOSI) | (1 << PIN_SS);

    // SPI Enable, Master mode
    SPCR |= (1 << SPE) | (1 << MSTR)| (1<<SPR1);

    // Decode mode to "Font Code-B"
    
    MAX7219_writeData(MAX7219_MODE_DECODE, 0xFF);

    // Scan limit runs from 0.
    MAX7219_writeData(MAX7219_MODE_SCAN_LIMIT, digitsInUse - 1);
    MAX7219_writeData(MAX7219_MODE_INTENSITY, 8);
    MAX7219_writeData(MAX7219_MODE_POWER, ON);
}

void spiSendByte (char databyte)
{
    // Copy data into the SPI data register
    SPDR = databyte;
    // Wait until transfer is complete
    while (!(SPSR & (1 << SPIF)));
}

void MAX7219_writeData(char data_register, char data)
{
    MAX7219_LOAD0;
        // Send the register where the data will be stored
        spiSendByte(data_register);
        // Send the data to be stored
        spiSendByte(data);
    MAX7219_LOAD1;
}

void MAX7219_clearDisplay() 
{
    char i = digitsInUse;
    // Loop until 0, but don't run for zero
    do {
        // Set each display in use to blank
        MAX7219_writeData(i, MAX7219_CHAR_BLANK);
    } while (--i);
}
//
void MAX7219_displayNumber(volatile long number) 
{
    char negative = 0;

    // Convert negative to positive.
    // Keep a record that it was negative so we can
    // sign it again on the display.
    if (number < 0) {
        negative = 1;
        number *= -1;
    }

    MAX7219_clearDisplay();

    // If number = 0, only show one zero then exit
    if (number == 0) {
        MAX7219_writeData(MAX7219_DIGIT0, chars);
        return;
    }
    
    // Initialization to 0 required in this case,
    // does not work without it. Not sure why.
    char i = 0; 
   //  MAX7219_writeData(++i,0b00000011);
                          
    // Loop until number is 0.
    do {
      
        MAX7219_writeData(++i, number % 10);
        number /= 10;

    } while (number);

    // Bear in mind that if you only have three digits, and
    // try to display something like "-256" all that will display
    // will be "256" because it needs an extra fourth digit to
    // display the sign.
    if (negative) {
        MAX7219_writeData(i, MAX7219_CHAR_NEGATIVE);
    }
}

//void MAX7219_displayNumberyMenu(volatile long number,int numero) 
//{
//    char negative = 0;
//
//    // Convert negative to positive.
//    // Keep a record that it was negative so we can
//    // sign it again on the display.
//    if (number < 0) {
//        negative = 1;
//        number *= -1;
//    }
//
//    MAX7219_clearDisplay();
//
//    // If number = 0, only show one zero then exit
//    if (number == 0) {
//        MAX7219_writeData(MAX7219_DIGIT0, 0);
//        return;
//    }
//    
//    // Initialization to 0 required in this case,
//    // does not work without it. Not sure why.
//    char i = 0; 
//    
//    // Loop until number is 0.
//    do {
//        MAX7219_writeData(++i, number % 10);
//        // Actually divide by 10 now.
//        number /= 10;
//    } while (number);
//    
//    MAX7219_writeData(MAX7219_DIGIT6, numero);
//    // Bear in mind that if you only have three digits, and
//    // try to display something like "-256" all that will display
//    // will be "256" because it needs an extra fourth digit to
//    // display the sign.
//    
//    if (negative) {
//        MAX7219_writeData(i, MAX7219_CHAR_NEGATIVE);
//    }
//}