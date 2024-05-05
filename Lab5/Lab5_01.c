unsigned char col = 0x01;
unsigned char row;
unsigned char key;

void initKeyPad()
{
    // --- set PD0~7 to connect to Keypad ---
    // COL1-4 as PORTD0-3  --> Output
    // ROW1-4 as PORTD4-7  --> Input
    DDRD |= 0x0F; // Output PD0~3 to LED as Col (Input from Keypad)
  	PORTD |= 0xF0; // Row (pull up setting)

  	// --- led command (setting output) ---
    // set PB0~3 as Column Output
    DDRB |= 0x0F;
    // clear PORTB0~3
  	PORTB &= 0xF0; 
    // set PC0~3 as Row Output
    DDRC |= 0x0F;
  	// clear PORTC0~3
    PORTC &= 0xF0;
}


void setup()
{
  initKeyPad();
}

void loop()
{
 	while(1)
 	{ 
      	// Mark with 11110000
       	PORTD = 0xF0 | ~(col & 0x0F);
      	// 00000001 & 00001111 -> 00000001 | 11110000 -> 11110001
        // 00000010 & 00001111 -> 00000010 | 11110000 -> 11110010
      	_delay_ms(20);
      	row = (PIND & 0xF0);
      	// 11010010 & 11110000	-> 11010000
      
      	// Check is key was pressed if yes row will be 
     	// 		- 1110000
      	// 		- 1101000
      	// 		- 1011000
      	// 		- 0111000
        // So break the loops while
      	if (row != 0xF0){
       		break; 
      	}
      	
      	// Shift column until 4 time shift and then reset
     	col = (col << 1); // 00000001 -> 00000010
      	if (col == 0x10){
        	col = 0x01;
      	}
  	}
  
  	//PORTB = 0xF0 | ~(col & 0x0F);
    PORTB = PORTD;
  	PORTC = 0xF0 | (row >> 4);
}