#ifndef SSD1306_LIB_NEW_H
#define SSD1306_LIB_NEW_H

#include "oled_demo.h"

// init the display
void LCD_init(void);
// is it command or is it data sending to the display
void LCD_Commmand(uint8_t ControByte, uint8_t DataByte);
// goto coordinates on the screen
void LCD_Goto(uint8_t x, uint8_t y);
// output picture from FLASH
void LCD_DrawImage(const uint8_t* image);
// setup contrast 0...255
void LCD_Contrast(uint8_t set_contrast);
// output of BIG digits and symbols
void LCD_BigNum(uint8_t num);
// output of the symbol (doublewidth=1 - double width)
void LCD_Char(uint8_t c, uint8_t doublewidth);
// type the string (size = 0 - small, 1 - middle, 2 - HUGE (digits only))
void LCD_Printf(char* buf, uint8_t size);

uint8_t LCD_X, LCD_Y;
uint8_t tx_buffer[129];

#define SSD1306_I2C_ADDRESS 0x78 << 1
#define SSD1306_I2C_H_ADDRESS 0x3C << 1
// Size
#define SSD1306_LCDWIDTH 128
#define SSD1306_LCDHEIGHT 64
// Commands
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR 0x22
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2
// Scrolling #defines
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERT_AND_RIGHT_HORIZ_SCROLL 0x29
#define SSD1306_VERT_AND_LEFT_HORIZ_SCROLL 0x2A

#define COMAND 0x00
#define DATA 0x40

#endif
