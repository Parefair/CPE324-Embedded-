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