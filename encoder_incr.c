#include "include/encoder_incr.h"

uint8_t PB4_B = 1, PB5_B = 1, enc_counter = 0;
uint8_t led_on = 0;

void EncoderIncrInit(void) {
    PCMSK0 = (1 << PCINT5) | (1 << PCINT4);
    PCMSK1 = (1 << PCINT12);
    PCICR = (1 << PCIE1) | (1 << PCIE0);
}

ISR(PCINT0_vect) {
    if((PINB & (1 << 4)) == 0) {
        if(((PINB & (1 << 5)) != 0) && (PB5_B == 1))
            enc_counter--;
    } else {
        if(((PINB & (1 << 5)) == 0) && (PB4_B == 1))
            enc_counter++;
    }
    if(PINB & (1 << 4))
        PB4_B = 1;
    else
        PB4_B = 0;
    if(PINB & (1 << 5))
        PB5_B = 1;
    else
        PB5_B = 0;
}

ISR(PCINT1_vect) {
    if((PINJ & (1 << 3)) > 0) {
        if(led_on > 0)
            led_on = 0;
        else
            led_on = 1;
    }
}