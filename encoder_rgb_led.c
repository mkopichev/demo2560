#include "include/encoder_rgb_led.h"

uint8_t PB4_B = 1, PB5_B = 1, enc_counter = 0;
uint8_t GLOBAL_GRB[3] = {0, 0, 0}, led_on = 0;

// for RGB_LED 11,0592 MHz
void Led(uint8_t red, uint8_t green, uint8_t blue) {
    for(uint8_t i = (1 << 7); i >= 1; i >>= 1) // Green
    {
        if((green & i) > 0) {
            PORTA |= (1 << 7); // should be 0.9us
            _NOP();
            _NOP();
            _NOP();
            _NOP();
            _NOP();
            _NOP();
            _NOP();
            PORTA &= ~(1 << 7);
        } // should be 0.35us but ??1.35us also works ok// now it's dick knows
        else {
            PORTA |= (1 << 7); // should be 0.35us but 0.4
            _NOP();
            PORTA &= ~(1 << 7);
        } // should be 0.9us but 1.35us also works ok
    }
    for(uint8_t i = (1 << 7); i >= 1; i >>= 1) // Red
    {
        if((red & i) > 0) {
            PORTA |= (1 << 7); // should be 0.9us
            _NOP();
            _NOP();
            _NOP();
            _NOP();
            _NOP();
            _NOP();
            _NOP();
            PORTA &= ~(1 << 7);
        } // should be 0.35us but 1.35us also works ok

        else {
            PORTA |= (1 << 7); // should be 0.35us but 0.4
            _NOP();
            PORTA &= ~(1 << 7);
        } // should be 0.9us but 1.35us also works ok
    }
    for(uint8_t i = (1 << 7); i >= 1; i >>= 1) // Blue
    {
        if((blue & i) > 0) {
            PORTA |= (1 << 7); // should be 0.9us
            _NOP();
            _NOP();
            _NOP();
            _NOP();
            _NOP();
            _NOP();
            _NOP();
            PORTA &= ~(1 << 7);
        } // should be 0.35us but 1.35us also works ok
        else {
            PORTA |= (1 << 7); // should be 0.35us but 0.4
            _NOP();
            PORTA &= ~(1 << 7);
        } // should be 0.9us but 1.35us also works ok
    }
}

void hue_to_rgb(uint8_t hue) {
    if(hue <= 85) {
        GLOBAL_GRB[2] = (85 - hue) * 3; // GLOBAL_GRB[0]RB[2]
        GLOBAL_GRB[1] = hue * 3;
        GLOBAL_GRB[0] = 0;
        return;
    }
    if(hue > 170) {
        GLOBAL_GRB[2] = (hue - 171) * 3;
        GLOBAL_GRB[1] = 0;
        GLOBAL_GRB[0] = (255 - hue) * 3;
        return;
    } else {
        GLOBAL_GRB[2] = 0;
        GLOBAL_GRB[1] = (171 - hue) * 3;
        GLOBAL_GRB[0] = (hue - 86) * 3;
        return;
    }
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