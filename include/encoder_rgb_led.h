#ifndef ENCODER_RGB_LED_H
#define ENCODER_RGB_LED_H

#include <avr/cpufunc.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>

extern uint8_t PB4_B, PB5_B, enc_counter;
extern uint8_t GLOBAL_GRB[3], led_on;

void Led(uint8_t red, uint8_t green, uint8_t blue);
void hue_to_rgb(uint8_t hue);

#endif