#ifndef FONTS_PICTURES_OLED_H
#define FONTS_PICTURES_OLED_H

#include <avr/pgmspace.h>
#include <stdint.h>

extern const uint8_t PROGMEM arrow[10];
extern const char PROGMEM LCD_Buffer[0x0500];
extern const char PROGMEM BigNum[15][96];

// 7 running dog pictures
extern const uint8_t PROGMEM dog0[1024];
extern const uint8_t PROGMEM dog1[1024];
extern const uint8_t PROGMEM dog2[1024];
extern const uint8_t PROGMEM dog3[1024];
extern const uint8_t PROGMEM dog4[1024];
extern const uint8_t PROGMEM dog5[1024];
extern const uint8_t PROGMEM dog6[1024];
// end of the running dog pictures

// jumping spitz
extern const uint8_t PROGMEM spitz0[1024];
extern const uint8_t PROGMEM spitz1[1024];
extern const uint8_t PROGMEM spitz2[1024];
extern const uint8_t PROGMEM spitz3[1024];
extern const uint8_t PROGMEM spitz4[1024];
extern const uint8_t PROGMEM spitz5[1024];
extern const uint8_t PROGMEM spitz6[1024];
extern const uint8_t PROGMEM spitz7[1024];
extern const uint8_t PROGMEM spitz8[1024];
extern const uint8_t PROGMEM spitz9[1024];
extern const uint8_t PROGMEM spitz10[1024];
// end of jumping spitz

extern const uint8_t PROGMEM tiger_image[1024];
extern const uint8_t PROGMEM avr_image[1024];
extern const uint8_t PROGMEM mishaniya[1024];

#endif
