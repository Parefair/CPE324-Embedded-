/*
 * File:   main.c
 * Author: kanyaluckchimchome
 *
 * Created on March 6, 2024, 11:32 AM
 */
#define F_CPU 8000000L

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

char buffer[16];

void commitData()
{
  PORTD |= (1 << PORTD4);// Set Enable to high
  _delay_us(2000);
  PORTD &= ~(1 << PORTD4);// Set Enable to low
  _delay_us(2000);
}

void sendLCDCommand(uint8_t command)
{
  _delay_us(100);
  // pull RS Low for commanding
  PORTD &= ~(1 << PORTD2);
  
  // send high nibble (4 bits) of the command
  PORTC &= 0xF0; // clear 4 bits แรกให้เป็น 0
  PORTC |= command >> 4;
  commitData();
  
  // send low nibble (4 bits) of the command
  PORTC &= 0xF0; // clear 4 bits แรกให้เป็น 0
  PORTC |= command & 0x0F; // 4 bits บน ถูกบังคับเป็น 0 ส่วน 4 bits ล่างมีค่าเหมือนเดิม
  commitData();
}

void sendLCDData(uint8_t command)
{
  _delay_us(100);
  // pull RS High for writing
  PORTD |= (1 << PORTD2);
  
  // send high nibble (4 bits) of the command
  PORTC &= 0xF0; // clear 4 bits ล่างให้เป็น 0
  PORTC |= command >> 4;
  commitData();
  
  // send low nibble (4 bits) of the command
  PORTC &= 0xF0; // clear 4 bits ล่างให้เป็น 0
  PORTC |= command & 0x0F; // 4 bits บน ถูกบังคับเป็น 0 ส่วน 4 bits ล่างมีค่าเหมือนเดิม
  commitData();
}

void lcdDisplayString(char* str)
{
  while (*str != '\0')
  {
    sendLCDData(*str);
    str++;
  }
}

void initLCD()
{
	// Set PC0~PC3 as output
  	DDRC |= 0x0F;
  	// Clear PC0~PC3 as 0
  	PORTC &= 0xF0; //clear 4 bit หลังเป็น 0
  
  	// Set PD2(RS) and PD4(E) as output
  	DDRD |= (1 << DDD2) | (1 << DDD4);
  	// Clear PD2 and PD4 
  	PORTD &= ~(1 << PORTD2) & ~(1 << PORTD4);
  
  	sendLCDCommand(0x33);
  	sendLCDCommand(0x32);
  	sendLCDCommand(0x28);
  	sendLCDCommand(0x0E);
  	sendLCDCommand(0x01);
  	sendLCDCommand(0x80);
}

void clearLCD()
{
    sendLCDCommand(0x01); 
    _delay_ms(2);
    sendLCDCommand(0x80);  
}

void initADC()
{
  	// Set ADMUX
  	//  - choose ADC5 [0101]
    //  - choose AVcc to be reference voltage -> ADMUX |= (1 << REFS0);
    ADMUX = 0b01000101;
  	
  	// Set ADCSRA 
  	//  - enable ADC
  	//  - set clock Divider factor (frequency must between 50 - 200 kHz)
  	ADCSRA |= (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
  
  	// Set ADCSRB
  	// because we use free running mode so choose 0000 -> Don't have to set
}

void setup()
{
    // Serial.begin(9600);
    
    // Init LCD
    initLCD();
    
    // Init ADC
    initADC();
    
    // enable ADC interupt and start conversion
    ADCSRA |= (1 << ADIE) | (1 << ADSC);
    
    // Don't need to set ADCSRB because we use free running mode so choose 0000 -> Don't have to set
   
    // enable interupt
    sei();
}

ISR (ADC_vect) 
{
    // Clear screen
    clearLCD();
  	
  	uint16_t adcValue = ADC;
  
  	// use sprintf to convert from int to string
    sprintf(buffer, "%d", adcValue);
  	lcdDisplayString(buffer);
  
  	_delay_ms(300);
    
    // Start Conversion
    ADCSRA |= (1 << ADSC);
}

void loop()
{
    
}

int main (void)
{
    setup();
   
    while(1){
        loop();
    }
}