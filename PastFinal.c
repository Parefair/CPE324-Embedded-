int n = 0;
char buffer[16];
uint16_t timercount;
uint16_t tempcount;

// 0 - off, 1 - on
int status = 0;

int time = 0; // Count time continuously
int start_time; 
int stop_time;  

void commitData()
{
  PORTD |= (1 << PORTD1);// Set Enable to high
  _delay_us(10);
  PORTD &= ~(1 << PORTD1);// Set Enable to low
  _delay_us(10);
}


void sendLCDCommand(uint8_t command)
{
  _delay_us(100);
  // pull RS Low for commanding
  PORTD &= ~(1 << PORTD0);
  
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
  _delay_us(100);
  // pull RS High for writing
  PORTD |= (1 << PORTD0);
  
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
  	DDRD |= (1 << DDD0) | (1 << DDD1);
  	// Clear PD2 and PD4 
  	PORTD &= ~(1 << PORTD0) & ~(1 << PORTD1);
  
  	sendLCDCommand(0x33);
  	sendLCDCommand(0x32);
  	sendLCDCommand(0x28);
  	sendLCDCommand(0x0E);
  	sendLCDCommand(0x01);
  	sendLCDCommand(0x80);
}

void clearLCD()
{
    sendLCDCommand(0x01); 
    _delay_ms(2);
    sendLCDCommand(0x80);  
}

ISR (INT0_vect) 
{
   	// button is free
    if (PIND & (1 << PORTD2)){
        stop_time = time;

        // button is pressed longer than 3 second = Reset to zero
        if ((stop_time - start_time >= 3)){
            status = 0;
          	timercount = 0;
        }
        
        else{
            if (status == 1){
              timercount = tempcount;
              status = 0;
  			}
            else{
              timercount += 5;
              tempcount = timercount;
            }
        }
    }
    // button is pressed
    else{
        start_time = time;
    }
}

ISR (INT1_vect) 
{
    status = !status;
}

void initInterupt(){
  
  	// the rising and falling edge of interupt INT0 generate the interupt request
 	// the falling edge of interupt INT1 generate the interupt request
    EICRA |= (1 << ISC00) | (1 << ISC11); 
    
    // Enable external interupt 0 and 1
    EIMSK |= (1 << INT0) | (1 << INT1);
            
    // enable interupt
    sei(); 
}

ISR (TIMER1_OVF_vect)
{
    clearLCD();
    sprintf(buffer,"%d",timercount);
  	lcdDisplayString(buffer);
  
    if (status){
  	timercount-=1;
  	}
  
    time++;
    TCNT1 = (65536/8.5)*6.5; // 1 sec
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
    TCNT1 = (65536/8.5)*6.5; // 1 second
    
}

int main(){
  // Button Input
  PORTD |= (1 << PORTD2) |  (1 << PORTD3);
  
  initLCD();
  initInterupt();
  initTimer();
  
  while(1){
    
  }
}