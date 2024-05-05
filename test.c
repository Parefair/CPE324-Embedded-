/*
 * File:   main.c
 * Author: kanyaluckchimchome
 *
 * Created on March 6, 2024, 11:32 AM
 */
#define F_CPU 8000000L

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    
    // setup
    DDRB |= (1 << DDB1);
    
    while (1) {
        PORTB |= (1 << PORTB1);
        _delay_ms(1000);
        PORTB &= ~(1 << PORTB1);
        _delay_ms(1000);
    }
}
