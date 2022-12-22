#ifndef OLED_DEMO_H
#define OLED_DEMO_H

#include "encoder_incr.h"
#include "fonts_pictures_oled.h"
#include "i2c_basics.h"
#include "ssd1306_lib_new.h"
#include "ws2812b_rgb.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

void demo_oled(void);

#endif
