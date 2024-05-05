void commitData()
{
  PORTD |= (1 << PORTD4);// Set Enable to high
  _delay_us(10);
  PORTD &= ~(1 << PORTD4);// Set Enable to low
  _delay_us(10);
}


void sendLCDCommand(uint8_t command)
{
  // pull RS Low for commanding
  PORTD &= ~(1 << PORTD2);
  
  // send high nibble (4 bits) of the command
  PORTB &= 0xF0; // clear 4 bits แรกให้เป็น 0
  PORTB |= command >> 4;
  commitData();
  
  // send low nibble (4 bits) of the command
  PORTB &= 0xF0; // clear 4 bits แรกให้เป็น 0
  PORTB |= command & 0x0F; // 4 bits บน ถูกบังคับเป็น 0 ส่วน 4 bits ล่างมีค่าเหมือนเดิม
  commitData();
}

void sendLCDData(uint8_t command)
{
  // pull RS High for writing
  PORTD |= (1 << PORTD2);
  
  // send high nibble (4 bits) of the command
  PORTB &= 0xF0; // clear 4 bits ล่างให้เป็น 0
  PORTB |= command >> 4;
  commitData();
  
  // send low nibble (4 bits) of the command
  PORTB &= 0xF0; // clear 4 bits ล่างให้เป็น 0
  PORTB |= command & 0x0F; // 4 bits บน ถูกบังคับเป็น 0 ส่วน 4 bits ล่างมีค่าเหมือนเดิม
  commitData();
}

void lcdDisplayString(char* str)
{
  int i=0;
  // Sol 1 แต่ มันเปลืองพื้นที่ต้องลง lib เพิ่ม จึงใช้ Sol 2
  /*
  for (i=0; i<strlen(str); i++)
  {
    sendLCDData(str[i]);
  }
  */
  // Sol 2
  while (*str != '\0')
  {
    sendLCDData(*str);
    str++;
  }
}

void initLCD()
{
	// Set PB0~PB3 as output
  	DDRB |= 0x0F;
  	// Clear PB0~PB3 as 0
  	PORTB &= 0xF0; //clear 4 bit หลังเป็น 0
  
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

void setup()
{
	initLCD();
  	sendLCDData('H');
  	sendLCDData('E');
  	sendLCDData('L');
  	sendLCDData('L');
  	sendLCDData('O');
  	sendLCDCommand(0xC0);
  	lcdDisplayString("CPE324");
}

void loop()
{
  
}