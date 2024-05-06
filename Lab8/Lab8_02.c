#define F_CPU 8000000L

//#include <iom328p.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void commitData()
{
  PORTD |= (1 << PORTD4); // Set Enable to high
  _delay_us(2000);
  PORTD &= ~(1 << PORTD4); // Set Enable to low
  _delay_us(2000);
}


void sendLCDCommand(uint8_t command)
{
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
  int i=0;
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

void initUltrasonic()
{
 	// trigger as output at PC5
  	DDRC |= (1 << DDC5);
  	// Clear output port
  	PORTC &= ~(1 << PORTC5);
  	// echo as input at PB0
  	DDRB &= (1 << DDB0);
  	PORTB |= (1 << PORTB0);
}

char buffer[32];

void initCapture(){
    TCCR1B |= (1 << ICNC1) | (1 << CS10) | (1 << WGM12);
}

int TimerOverflow = 0;

ISR(TIMER1_OVF_vect)
{
	TimerOverflow++;	//Increse Timer Overflow count
}

int main()
{
  // Serial.begin(9600);
  long count;
  double distance;
 
  // Init LCD
  initLCD();
  
  // Init Ultrasonic Sensor
  initUltrasonic();
  
  // Init Capture
  initCapture();
  
  // Enable Global Interrupts
  sei();
  
  TIMSK1 = (1 << TOIE1);	// Enable Timer1 overflow interrupts
  TCCR1A = 0;	
  while(1){

    sendLCDCommand(0x01);
    sendLCDCommand(0x80);
    lcdDisplayString("Dist : ");

    PORTC |= (1 << PORTC5);
    _delay_us(10);
    PORTC &= (~(1 << PORTC5));
    
    // Set timer
    TCNT1 = 0;	// Clear Timer
		TCCR1B = 0x41;	// Rising edge & No prescaler
		TIFR1 = 1<<ICF1;	// Clear ICP flag 
		TIFR1 = 1<<TOV1;	// Clear Timer Overflow flag

		//Calculate width of Echo by Input Capture (ICP) 
		
		while ((TIFR1 & (1 << ICF1)) == 0); // Loop to wait for rising edge
		TCNT1 = 0;	// Clear Timer counter 
		TCCR1B = 0x01;	// Capture on falling edge, No prescaler
		TIFR1 = 1<<ICF1;
		TIFR1 = 1<<TOV1;	
		TimerOverflow = 0; // Clear Timer overflow count

		while ((TIFR1 & (1 << ICF1)) == 0); // Loop to wait for falling edge
		count = ICR1 + (65535 * TimerOverflow);	

		// 8MHz Timer freq
    // sound speed =343 m/s
		distance = (double)count / 466.47;
        
    char diff[16];
    sprintf(diff, "%.2lf", distance);
    lcdDisplayString(diff);
    
    lcdDisplayString("cm");
    
    _delay_ms(500);

    }
}