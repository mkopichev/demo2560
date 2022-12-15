#ifndef FONTS_PICTURES_OLED_H
#define FONTS_PICTURES_OLED_H

#include <stdint.h>
#include <avr/pgmspace.h>

extern const uint8_t PROGMEM arrow[10];
extern const uint8_t PROGMEM LCD_Buffer[0x0500];
extern const uint8_t PROGMEM BigNum[15][96];

//7 running dog pictures
extern const uint8_t PROGMEM dog0[1024];
extern const uint8_t PROGMEM dog1[1024];
extern const uint8_t PROGMEM dog2[1024];
extern const uint8_t PROGMEM dog3[1024];
extern const uint8_t PROGMEM dog4[1024];
extern const uint8_t PROGMEM dog5[1024];
extern const uint8_t PROGMEM dog6[1024];
//end of the running dog pictures 
extern const uint8_t PROGMEM mishaniya[1024];

#endif
