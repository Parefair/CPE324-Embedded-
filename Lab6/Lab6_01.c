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

void setup()
{
    // Serial.begin(9600);
    
    // LED output
    DDRB |= (1 << DDB0);
    
    // Button input enable pull up
    PORTD |= (1 << PORTD2);
    
    // the falling edge of interupt INT0 genrate the interupt request
    EICRA |= (1 << ISC01); 
    
    // Enable external interupt 0
    EIMSK |= (1 << INT0);
            
    // enable interupt
    sei();
}

ISR (INT0_vect) 
{
    state = !state;
    // turn off LED (cause by button interupt)
    if (!state)
    {
        PORTB &= ~(1 << PORTB0);
    }
}

void loop()
{
    if (state)
    {
        // toggle LED
        PORTB ^= (1 << PORTB0); 
    }
    // wait 2 second
    _delay_ms(1000);
}

int main (void)
{
    setup();
    while(1){
        loop();
    }
}