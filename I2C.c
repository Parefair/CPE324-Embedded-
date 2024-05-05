void i2c_init(void)
{
    TWSR &= ~((1 << TWPS1) | (1 << TWPS0)); // Set TWI prescaler = 1
    TWBR = 0x48; // Set i2c clock = 100kHz and crystal freq = 16 MHz
    TWCR |= (1 << TWEN); // Enable TWI interface
}

void i2c_start(void)
{
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); // write 1 to TWINT to reset Flag
    while ((TWCR&(1 << TWINT)) == 0); // Wait for TWINT return to 1
}

void i2c_write(uint8_t data)
{
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while ((TWCR & (1 << TWINT)) == 0);
}

unsigned char i2c_read(uint8_t ackVal)
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (ackVal << TWEA); //ackVal(0) = read once, ackVal(1) = multiple read
    while((TWCR & (1 << TWINT)) == 0);
    return TWDR;
}

void i2c_stop()
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void rtc_init(void)
{
    // Initialize I2C
    i2c_init();
    _delay_us(30);
    
    // start I2C
    i2c_start();
    
    // Write control byte
    i2c_write(0xD0); // 1101 0000 DS1307's Address
    i2c_write(0x07); // 0000 0111 Control
    i2c_write(0x00);
    
    // stop I2C
    i2c_stop();
}

void rtc_setDateTime(uint8_t year, uint8_t month, uint8_t date, uint8_t hour, uint8_t minute, uint8_t second)
// e.g. rtc_setTime(0x23, 0x59, 0x50) = 23:59:50
{
    // start I2C
    i2c_start();
    
    // Write control byte
    i2c_write(0xD0); // 1101 0000 DS1307's Address
    
    // Write Memory Address
    i2c_write(0x00);
    
    // Write Date and time
    i2c_write(second);
    i2c_write(minute);
    i2c_write(hour);
    
    // Stop I2C
    i2c_stop();
    
    // start I2C
    i2c_start();
    
    // Write control byte
    i2c_write(0xD0); // 1101 0000 DS1307's Address
    
    // Write Memory Address
    i2c_write(0x04);
    
    // Write Date and time
    i2c_write(date);
    i2c_write(month);
    i2c_write(year);
    
    // Stop I2C
    i2c_stop();
}

void rtc_getDateTime(uint8_t *year, uint8_t *month, uint8_t *date, uint8_t *hour, uint8_t *minute, uint8_t *second)
// e.g. rtc_getTime(&i, &j, &k);
// then lcdData_BCD(i);
// then lcdData(':')
// then ...
{
    // Start I2C
    i2c_start();
    
    // Write control byte
    i2c_write(0xD0); // 1101 0000 DS1307's Address
    
    // Write memory address
    i2c_write(0x00);
    
    // Stop I2C
    i2c_stop();
    
    // Start I2C
    i2c_start();
    
    // Write control byte
    i2c_write(0xD1); // 0xD1 for read
    
    *second = i2c_read(1); //multiple reading
    *minute = i2c_read(1); // still reading
    *hour = i2c_read(0); 
    
    // Stop I2C
    i2c_stop();
    
    // Start I2C
    i2c_start();
    
    // Write control byte
    i2c_write(0xD0); // 1101 0000 DS1307's Address
    
    // Write memory address
    i2c_write(0x04);
    
    // Stop I2C
    i2c_stop();
    
    // Start I2C
    i2c_start();
    
    // Write control byte
    i2c_write(0xD1); // 0xD1 for read
    
    *date = i2c_read(1);
    *month = i2c_read(1); 
    *year = i2c_read(0); // finish reading
    
    // Stop I2C
    i2c_stop();
}

void display_month(uint8_t month) {
    char str[10];
    switch (month) {
        case 0x01:
            sprintf(str, "Jan");
            lcdDisplayString(str);
            break;
        case 0x02:
            sprintf(str, "Feb");
            lcdDisplayString(str);
            break;
        case 0x03:
            sprintf(str, "Mar");
            lcdDisplayString(str);
            break;
        case 0x04:
            sprintf(str, "Apr");
            lcdDisplayString(str);
            break;
        case 0x05:
            sprintf(str, "May");
            lcdDisplayString(str);
            break;
        case 0x06:
            sprintf(str, "Jun");
            lcdDisplayString(str);
            break;
        case 0x07:
            sprintf(str, "Jul");
            lcdDisplayString(str);
            break;
        case 0x08:
            sprintf(str, "Aug");
            lcdDisplayString(str);
            break;
        case 0x09:
            sprintf(str, "Sep");
            lcdDisplayString(str);
            break;
        case 0x10:
            sprintf(str, "Oct");
            lcdDisplayString(str);
            break;
        case 0x11:
            sprintf(str, "Nov");
            lcdDisplayString(str);
            break;
        case 0x12:
            sprintf(str, "Dec");
            lcdDisplayString(str);
            break;
        default:
            sprintf(str, "Err");
            lcdDisplayString(str);
            break;
    }
}