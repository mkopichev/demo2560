#ifndef ENCODER_INCR_H
#define ENCODER_INCR_H

#include "ws2812b_rgb.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>

extern uint8_t PB4_B, PB5_B, enc_counter;
extern uint8_t led_on;

void EncoderIncrInit(void);

#endif