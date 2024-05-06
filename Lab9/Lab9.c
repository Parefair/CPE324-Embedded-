#define F_CPU 8000000L

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

char buffer[16];


/* Define LCD Port */
#define LCD_DATA_PORT PORTC
#define LCD_DATA_DDR DDRC
#define LCD_CTRL_PORT PORTD
#define LCD_CTRL_DDR DDRD

#define RS PD2
#define E PD4
#define D4 PC0
#define D5 PC1
#define D6 PC2
#define D7 PC3

char buffer[16];

void commitData()
{
  LCD_CTRL_PORT |= (1 << E);// Set Enable to high
  _delay_us(2000);
  LCD_CTRL_PORT &= ~(1 << E);// Set Enable to low
  _delay_us(2000);
}

void sendLCDCommand(uint8_t command)
{
  _delay_us(100);
  // pull RS Low for commanding
  LCD_CTRL_PORT &= ~(1 << RS);
  
  // send high nibble (4 bits) of the command
  LCD_DATA_PORT &= 0xF0; // clear 4 bits แรกให้เป็น 0
  LCD_DATA_PORT |= command >> 4;
  commitData();
  
  // send low nibble (4 bits) of the command
  LCD_DATA_PORT &= 0xF0; // clear 4 bits แรกให้เป็น 0
  LCD_DATA_PORT |= command & 0x0F; // 4 bits บน ถูกบังคับเป็น 0 ส่วน 4 bits ล่างมีค่าเหมือนเดิม
  commitData();
}

void sendLCDData(uint8_t command)
{
  _delay_us(100);
  // pull RS High for writing
  LCD_CTRL_PORT |= (1 << RS);
  
  // send high nibble (4 bits) of the command
  LCD_DATA_PORT &= 0xF0; // clear 4 bits ล่างให้เป็น 0
  LCD_DATA_PORT |= command >> 4;
  commitData();
  
  // send low nibble (4 bits) of the command
  LCD_DATA_PORT &= 0xF0; // clear 4 bits ล่างให้เป็น 0
  LCD_DATA_PORT |= command & 0x0F; // 4 bits บน ถูกบังคับเป็น 0 ส่วน 4 bits ล่างมีค่าเหมือนเดิม
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
  	LCD_DATA_DDR |= 0x0F;
  	// Clear PC0~PC3 as 0
  	LCD_DATA_PORT &= 0xF0; //clear 4 bit หลังเป็น 0
  
  	// Set PD2(RS) and PD4(E) as output
  	LCD_CTRL_DDR |= (1 << RS) | (1 << E);
  	// Clear PD2 and PD4 
  	LCD_CTRL_PORT &= ~(1 << RS) & ~(1 << E);
  
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

void initSPI()
{
    //Enable SPI, Select to be Master and Set clock prescalar /8 (Max clock of MCP9700 is1.6MHz)
    SPCR |= (1 << SPE) | (1 << MSTR) | ( 1 << SPR0);
    SPSR |= (1 << SPI2X);
}

void setup()
{
    DDRB |= (1 << DDB2) | (1 << DDB5);
    PORTB |= (1 << PORTB2);
    
    initLCD();
    
    initSPI();
}  

void loop()
{
    clearLCD();
    
    PORTB &= ~(1 << PORTB2);
    
    // -------- High Byte ---------
    SPDR = 0;
    while (!(SPSR & (1 << SPIF)));
    uint8_t highByte = SPDR;
    // Clear Flag Manually (Because not turn on the interupt)
    SPSR |= (1 << SPIF);
    
    // -------- Low Byte ---------
    SPDR = 0;
    while (!(SPSR & (1 << SPIF)));
    uint8_t lowByte = SPDR;
    // Clear Flag Manually (Because not turn on the interupt)
    SPSR |= (1 << SPIF);
    
    PORTB |= (1 << PORTB2);
    
    uint16_t adcValue = (((highByte & 0x1F) << 8) | lowByte) >> 1;
    // Calculate temp from graph
    uint16_t temp = ((adcValue * 500.0) / 4095.0) - 50;
    
    sprintf(buffer, "%u", temp);
    lcdDisplayString("Temp: ");
    lcdDisplayString(buffer);
    _delay_ms(1000);
}

int main (void)
{
    setup();
    while(1){
        loop();
    }
}