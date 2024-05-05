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

int state = 1;

void initInterupt()
{
    // the falling edge of interupt INT0 genrate the interupt request
    EICRA |= (1 << ISC01);

    // Enable External Interupt 0
    EIMSK |= (1 << INT0);

    //Enable Global Interupt
    sei();
}

void initTimer()
{
    // Normal mode -> don't have to set anything
//    TCCR1A = 0x00;
    
    // Set Prescaler = 1024
  	TCCR1B |= (1 << CS12) | (1 << CS10);
    
    // Enable Timer Interupt
    TIMSK1 |= (1 << TOIE1);
    
    // 2^16 = 65536
    //65536 - (65536/4)
//    TCNT1 = 49152; 
    TCNT1 = (65536/8.5)*6.5;
    
}

ISR (TIMER1_OVF_vect)
{
    if (state)
    {
        PORTB ^= (1 << PORTB0);
        TCNT1 = (65536/8.5)*6.5;
    }
    if (!state)
    {
        PORTB &= ~(1 << PORTB0);
    }
}

ISR (INT0_vect)
{
    state = !state;
    // if state = 0 turn of LED
    if(!state)
    {
        TIMSK1 &= ~(1<<TOIE1);
        PORTB &= ~(1 << PORTB0);
    }
    if(state)
    {
        TIMSK1 |= (1<<TOIE1);
    }
}

void setup()
{
    // Serial.begin(9600);
    
    // PB1 LED output
    DDRB |= (1 << DDB0);
    
    // PB Button Input
    PORTD |= (1 << PORTD2);
    
    initTimer();
    
    initInterupt();
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