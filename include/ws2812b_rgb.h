#ifndef WS2812B_RGB_H
#define WS2812B_RGB_H

#include <avr/cpufunc.h>
#include <avr/io.h>
#include <stdint.h>

extern uint8_t GLOBAL_GRB[3];

void Ws2812bInit(void);
void Led(uint8_t red, uint8_t green, uint8_t blue);
void HueToRgb(uint8_t hue);

#endif