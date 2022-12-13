#ifndef OLED_DEMO_H
#define OLED_DEMO_H

#define I2C_hard 
//�����������, ���� ���� I2C

#ifdef I2C_hard
 #include <util/twi.h>
#else
 #include <i2c.h>
#endif

#include "i2c_basics.h"
#include "fonts_pictures_oled.h"
#include "ssd1306_lib_new.h" 

void demo_oled(void);

// extern const uint8_t arrow[10];
// extern const uint8_t LCD_Buffer[0x0500];
// extern const uint8_t BigNum[15][96];

// //7 running dog pictures
// extern const uint8_t dog0[1024];
// extern const uint8_t dog1[1024];
// extern const uint8_t dog2[1024];
// extern const uint8_t dog3[1024];
// extern const uint8_t dog4[1024];
// extern const uint8_t dog5[1024];
// extern const uint8_t dog6[1024];
// //end of the running dog pictures 

// //tiger
// extern const uint8_t image0[1024]; 
// //avr image
// extern const uint8_t image1[1024];
// extern const uint8_t mishaniya[1024];

#endif
