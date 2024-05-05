void setup()
{
  DDRB |= (1 << DDB3); // set PB3 as output
  PORTB |= (1 << PORTB4); //turn on internal pull up resister
  PORTB |= (1 << PORTB5); //turn on internal pull up resister
  
  while(1)
  {
    // Solution 1
    /*
    while ( (PINB & (1 << PINB5)) && (PINB & (1 << PINB4)) ); // wait for button press
  	_delay_ms(30);
    while ( !(PINB & (1 << PINB5) && ( PINB & (1 << PINB4))) ); // wait for button release
    _delay_ms(30);
    
    PORTB |= (1 << PINB3); // turn on led
    
    while ( (PINB & (1 << PINB5)) && (PINB & (1 << PINB4)) ); // wait for button press
  	_delay_ms(30);
    while ( !(PINB & (1 << PINB5) && ( PINB & (1 << PINB4))) ); // wait for button release
    _delay_ms(30);
    
    PORTB &= ~(1 << PINB3); // turn off led
    */
    
    // Solution 2
    while ( (PINB & (1 << PINB5)) && (PINB & (1 << PINB4)) ); // wait for button press
  	_delay_ms(30);
    while ( !(PINB & (1 << PINB5) && ( PINB & (1 << PINB4))) ); // wait for button release
    _delay_ms(30);
    
    PORTB ^= (1 << PINB3); // toggle the led
  }
 
}

void loop()
{
}