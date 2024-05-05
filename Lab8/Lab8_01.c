#define F_CPU 8000000L


#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

/*
 * 
 */
int main(void) {
  DDRD |= (1 << DDD6); // LED output
  
  // Init ADC
  ADMUX |= (1 << REFS0);
  ADCSRA |= (1 << ADEN)|(1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0);
  
  //PWM Fast Mode
  TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0A1);
  // Enable clock
  TCCR0B |= (1 << CS00);
 
  while (1){
        // start conversion
        ADCSRA |= (1 <<ADSC);

        // wait for ADIF or ADSC
        while(ADCSRA & (1 << ADSC)); // นำ mass มา & เพื่อเช็คแค่ bit ใด bit

        // 0-1023
        uint16_t adcValue = ADC;
        //Serial.println (adcValue);

        // make it in scope 0-1.0
        float duty = adcValue/1023.0;
        //Serial.println (duty);

        OCR0A = 255 * duty; // D6
        //_delay_ms(500);
   }
}