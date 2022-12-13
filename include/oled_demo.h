#define I2C_hard 
//�����������, ���� ���� I2C

#ifdef I2C_hard
 #include <util/twi.h>
#else
 #include <i2c.h>
#endif

#include "fonts_pictures_oled.h"
#include "ssd1306_lib_new.h" 