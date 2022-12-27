#include "include/i2c_basics.h"

uint8_t i2c_mutex = VACANT;

void I2cInit(void) {
    // I2C ports
    DDRD &= ~(1 << 0); // SCL
    DDRD &= ~(1 << 1); // SDA
    PORTD |= (1 << 0); // pull-up
    PORTD |= (1 << 1); // pull-up
    //TWBR = 67; or TWBR = (uint8_t)((F_CPU/(SCL_FREQ*1000))-16)/(2*4^TWPS);
    TWBR = (uint8_t)((F_CPU/SCL_FREQ)-16)/2; //100kHz 
}

void I2cStart(void) {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while(!(TWCR & (1 << TWINT))) continue;
}

void I2cStop(void) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void I2cTransmitByte(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while(!(TWCR & (1 << TWINT))) continue;
}

uint8_t I2cReceiveByte(uint8_t is_last_byte) {
    if(is_last_byte)
        TWCR = (1 << TWINT) | (1 << TWEN);
    else
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    while(!(TWCR & (1 << TWINT))) continue;
    return (TWDR);
}

void I2cAcquire(void) {
    while(1) {
        if(i2c_mutex == VACANT) {
            i2c_mutex = OCCUPIED;
            break;
        }
    }
}

void I2cRelease(void) {
    i2c_mutex = VACANT;
}