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

// State of switch
// 0 : off
// 1 : run led
// 2 : stop run led
int state = 0;

// for running LED
int order = 0; 

int timer = 0; // Count time continuously
int start_time; 
int stop_time;  

void close_all_led(){
    PORTB &= ~(1 << PORTB0);
    PORTB &= ~(1 << PORTB1);
    PORTB &= ~(1 << PORTB2);
}

void swap_light(uint8_t PORTXN){
    close_all_led();
    PORTB |= (1 << PORTXN);
}

ISR (TIMER1_OVF_vect)
{
    if (state == 1)
    {
        switch (order % 4){
            case 0:
                swap_light(PORTB0);
                break;
            case 1:
            case 3:
                swap_light(PORTB1);
                break;
            case 2:
                swap_light(PORTB2);
                break;
        }
        order++;
    }
    else if (state = 0){
        close_all_led();
    }
    timer++;
    TCNT1 = (65536/8.5)*7.5; // 1 sec
}

ISR (INT0_vect)
{
    // button is free
    if (PIND & (1 << PORTD2)){
        stop_time = timer;

        // button is pressed longer than 3 second = Close all LED
        if ((stop_time - start_time >= 2) && (state != 0)){
            close_all_led();
            state = 0;
        }
        // swap state between run and stop LED
        else{
            // if running -> stop it
            if (state == 1){
                state = 2;
            }
            // if it stop / off -> run it
            else{
                state = 1;
            }
        }
    }
    // button is pressed
    else{
        start_time = timer;
    }
}

void initInterupt()
{
    // the rising and falling edge of interupt INT0 genrate the interupt request
    EICRA |= (1 << ISC00);

    // Enable External Interupt 0
    EIMSK |= (1 << INT0);

    //Enable Global Interupt
    sei();
}

void initTimer()
{
    // Normal mode -> don't have to set anything
    TCCR1A = 0x00;
    
    // Set Prescaler = 1024
  	TCCR1B |= (1 << CS12) | (1 << CS10);
    
    // Enable Timer Interupt
    TIMSK1 |= (1 << TOIE1);
    
    // 2^16 = 65536
    TCNT1 = (65536/8.5)*7.5; // 1 second
    
}

void setup()
{
    // Serial.begin(9600);
    
    // PB0-2 LED output
    DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2);
    
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