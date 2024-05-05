void USART_Init(unsigned int ubrr){
  /* Set baud rate */
  UBRR0H = (unsigned char)(ubrr >> 8);
  UBRR0L = (unsigned char)ubrr;
  
  /* Enable receiver and transmitter */
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  
  /* Set frame format: 8 data, 2 stop bit */
  UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */ 
  	while (! ( UCSR0A & (1<<UDRE0) ) ) ;

	/* Put data into buffer, sends the data */
	UDR0 = data;
}
  
unsigned char USART_Receive() 
{
	/* Wait for data to be received */
  	while (! (UCSR0A & (1<<RXC0)) ) ;

	/* Get and return received data from buffer */ 
  	return UDR0;
}
  
void displayString(char* str)
{
  while (*str != '\0' && *str != '.')
  {
    USART_Transmit(*str);
    str++;
  }
}
  
void setup()
{
  //Set baud rate
  USART_Init(103);
  char buffer[30];
  int i;
  while(1){
  	unsigned char receivedChar;
    receivedChar = USART_Receive();
    buffer[i++] = receivedChar;
    
    if(receivedChar == '.'){
      displayString("Hello ");
      displayString(buffer);
      displayString("\n");
      // Clear Buffer
      memset(buffer,0,sizeof(buffer));
      i = 0;
    }
  }
}

void loop()
{
  
}