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

#endif
