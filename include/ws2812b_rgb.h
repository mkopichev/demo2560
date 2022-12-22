#ifndef WS2812B_RGB_H
#define WS2812B_RGB_H

#include <avr/cpufunc.h>
#include <avr/io.h>
#include <stdint.h>

extern uint8_t GLOBAL_GRB[3];

void Led(uint8_t red, uint8_t green, uint8_t blue);
void hue_to_rgb(uint8_t hue);

#endif