/*
Library for SSD1306 OLED display.
Initially is written for CVAVR.
22.12.2015 Vinichenko A.V.
29.07.2020 changed by Putov A.V.
16.12.2022 Changed for VS Code by Belskiy G.V.
*/

#include "include/ssd1306_lib_new.h"

void LCD_init(void) {
    // return;
    // delay_ms(10);
    LCD_Commmand(COMAND, SSD1306_SETDISPLAYCLOCKDIV);
    LCD_Commmand(COMAND, 0x80);

    LCD_Commmand(COMAND, SSD1306_SETMULTIPLEX);
    LCD_Commmand(COMAND, 0x3F);

    LCD_Commmand(COMAND, SSD1306_SETDISPLAYOFFSET);
    LCD_Commmand(COMAND, 0x00);

    LCD_Commmand(COMAND, SSD1306_SETSTARTLINE | 0x00);

    // We use internal charge pump
    LCD_Commmand(COMAND, SSD1306_CHARGEPUMP);
    LCD_Commmand(COMAND, 0x14);

    // Horizontal memory mode
    LCD_Commmand(COMAND, SSD1306_MEMORYMODE);
    LCD_Commmand(COMAND, 2);

    LCD_Commmand(COMAND, SSD1306_SEGREMAP | 0x1);

    LCD_Commmand(COMAND, SSD1306_COMSCANDEC);

    LCD_Commmand(COMAND, SSD1306_SETCOMPINS);
    LCD_Commmand(COMAND, 0x12);

    LCD_Commmand(COMAND, SSD1306_SETPRECHARGE);
    LCD_Commmand(COMAND, 0xF1);

    LCD_Commmand(COMAND, SSD1306_SETVCOMDETECT);
    LCD_Commmand(COMAND, 0x40);

    LCD_Commmand(COMAND, SSD1306_DISPLAYALLON_RESUME);

    // Non-inverted display
    LCD_Commmand(COMAND, SSD1306_NORMALDISPLAY);

    // Turn display on
    LCD_Contrast(255);
    LCD_Commmand(COMAND, SSD1306_DISPLAYON);

    LCD_Goto(0, 0);
}

void LCD_Commmand(uint8_t ControByte, uint8_t DataByte) {
    tx_buffer[0] = ControByte;
    tx_buffer[1] = DataByte;
    I2cStart();
    I2cTransmitByte(SSD1306_I2C_H_ADDRESS);
    for(uint8_t i = 0; i < 2; i++)
        I2cTransmitByte(tx_buffer[i]);
    I2cStop();
}

void LCD_Goto(uint8_t x, uint8_t y) {
    LCD_X = x;
    LCD_Y = y;
    // initial setup
    LCD_Commmand(COMAND, 0xB0 + y);        // set address of the coordinate`s initials
    LCD_Commmand(COMAND, x & 0x0f);        // LSB
    LCD_Commmand(COMAND, 0x10 | (x >> 4)); // MSB
}

void LCD_Contrast(uint8_t set_contrast) {
    LCD_Commmand(COMAND, SSD1306_SETCONTRAST);
    LCD_Commmand(COMAND, set_contrast);
}

// if (0) - clear screen
void LCD_DrawImage(const uint8_t *image) {
    uint16_t x;
    uint8_t tmp;
    for(tmp = 0; tmp < 8; tmp++) {
        LCD_Commmand(COMAND, 0xB0 + tmp); // set the coordinate`s initials address

        tx_buffer[0] = 0b01000000;
        for(x = 0; x < 128; x++) {
            if(image == 0) // clear
                tx_buffer[x + 1] = 0;
            else
                tx_buffer[x + 1] = pgm_read_byte(&image[tmp * 128 + x]);
        }
        I2cStart();
        I2cTransmitByte(SSD1306_I2C_H_ADDRESS);
        for(uint8_t i = 0; i < 129; i++)
            I2cTransmitByte(tx_buffer[i]);
        I2cStop();
    }
}

void LCD_Char(uint8_t c, uint8_t doublewidth) {
    uint8_t x = 0;

    tx_buffer[0] = DATA;
    if(doublewidth == 0) {
        for(x = 0; x < 5; x++) {
            tx_buffer[x + 1] = pgm_read_byte(&LCD_Buffer[c * 5 + x]);
        }
    } else {
        for(x = 0; x < 10; x++) {
            tx_buffer[x + 1] = pgm_read_byte(&LCD_Buffer[c * 5 + x / 2]);
        }
    }
    tx_buffer[x + 1] = 0;
    I2cStart();
    I2cTransmitByte(SSD1306_I2C_H_ADDRESS);
    for(uint8_t i = 0; i < (7 + doublewidth * 5); i++)
        I2cTransmitByte(tx_buffer[i]);
    I2cStop();
    if(doublewidth == 1)
        LCD_X += 11;
    else
        LCD_X += 8;
}

void LCD_BigNum(uint8_t num) {
    uint8_t x = 0;
    uint8_t z = 0;

    uint8_t y_s = LCD_Y;

    if(num != 46 && num != 32 && num != 32 && num != 67) {
        num = num - 48;
    } else {
        if(num == 46) {
            num = 11;
        } // .
        if(num == 32) {
            num = 12;
        } // space
        if(num == 67) {
            num = 13;
        } // degrees
    }

    for(z = 0; z < 4; z++) {
        tx_buffer[0] = DATA;
        for(x = 0; x < 24; x++) {
            tx_buffer[x + 1] = pgm_read_byte(&BigNum[num][z * 24 + x]);
        }
        I2cStart();
        I2cTransmitByte(SSD1306_I2C_H_ADDRESS);
        for(uint8_t i = 0; i < 25; i++)
            I2cTransmitByte(tx_buffer[i]);
        I2cStop();
        LCD_Goto(LCD_X, LCD_Y + 1);
    }

    LCD_X += 23;
    LCD_Goto(LCD_X, y_s);
}

void LCD_Printf(char *buf, uint8_t size) {

    while(*buf != 0) {
        if(size == 0) {
            LCD_Char(*buf++, 0);
        }
        if(size == 1) {
            LCD_Char(*buf++, 1);
        }
        if(size == 2) {
            LCD_BigNum(*buf++);
        }
    }
}

void LCD_DrawArrow(uint8_t del) {
    uint8_t x;
    tx_buffer[0] = 0b01000000;
    for(x = 0; x < 10; x++) {
        if(del == 0)
            tx_buffer[x + 1] = pgm_read_byte(&arrow[x]);
        else
            tx_buffer[x + 1] = 0;
    }
    I2cStart();
    I2cTransmitByte(SSD1306_I2C_H_ADDRESS);
    for(uint8_t i = 0; i < 11; i++)
        I2cTransmitByte(tx_buffer[i]);
    I2cStop();
}
