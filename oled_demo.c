/*******************************************************
SSD1306 OLED demo for VS Code atmega2560
ssd1306_lib_new.c - OLED library
fonts_pictures_oled.c - fonts and pictures

Display is using harware TWI.

Harware TWI works on 400k Hz speed.

To work with another controller or ports, enable and configure TWI in
your own projects (if programmatic). If less of flash output less pictures,
comment out the animation of the dog for instance.

Library is finalized for higher excecution speed.

Mishanja edition version :)

*******************************************************/

#include "include/oled_demo.h"

uint8_t PB4_B = 1, PB5_B = 1, enc_counter = 0, GLOBAL_GRB[3] = {0, 0, 0}, led_on = 0;

// 16 bit int to str uint8_t ?
void IntToStr(char *st, int16_t n) {
    uint8_t tmp2;
    int16_t tmp = 10000, tmpn, ind = 0;
    if(n < 0) {
        n = -n;
        st[ind] = '-';
        ind++;
    }

    tmpn = n;

    if(n == 0) {
        st[0] = 0 + '0';
        st[1] = 0;
        return;
    }

    while(tmp > 0) {
        if(tmp <= n) {
            tmp2 = tmpn / tmp;
            st[ind] = tmp2 + '0';
            tmpn = tmpn - tmp2 * tmp;
            ind++;
        }
        tmp = tmp / 10;
    }
    st[ind] = 0;
}

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

void demo_oled(void) {
    uint8_t tmp, counter = 0;
    char str[20];
    // // Crystal Oscillator division factor: 1
    // #pragma optsize-
    CLKPR = (1 << CLKPCE);
    CLKPR = (0 << CLKPCE) | (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);
    // #ifdef _OPTIMIZE_SIZE_
    // #pragma optsize+
    // #endif

    // I2C ports
    DDRD &= ~(1 << 0); // SCL
    DDRD &= ~(1 << 1); // SDA
    PORTD |= (1 << 0); // pull-up
    PORTD |= (1 << 1); // pull-up

    DDRA |= (1 << 7); // RGB_led
    PCMSK0 = (1 << PCINT5) | (1 << PCINT4);
    PCMSK1 = (1 << PCINT12);
    PCICR = (1 << PCIE1) | (1 << PCIE0);

    // TWI initialization
    // Mode: TWI Master
    // Bit Rate: 100 kHz
    I2cInit();

    _delay_ms(100);
    LCD_init();
    _delay_ms(1);
<<<<<<< HEAD

=======
    sei();
    
>>>>>>> 1a455a67f5c9c2aac8737bc42dff5ddbbd5610a3
    // dog animation
    // for(tmp = 0; tmp <= 4; tmp++) {
    //     LCD_DrawImage(spitz0);
    //     LCD_DrawImage(spitz1);
    //     LCD_DrawImage(spitz2);
    //     LCD_DrawImage(spitz3);
    //     LCD_DrawImage(spitz4);
    //     LCD_DrawImage(spitz5);
    //     LCD_DrawImage(spitz6);
    //     LCD_DrawImage(spitz7);
    // }
    // LCD_DrawImage(0); // CLEAR

    // LCD_Commmand(COMAND, SSD1306_INVERTDISPLAY);
    // LCD_DrawImage(mishaniya);
    // _delay_ms(3000);
    // LCD_Commmand(COMAND, SSD1306_NORMALDISPLAY);
    // LCD_DrawImage(0); // CLEAR

    // LCD_Goto(0, 1);
    // LCD_Printf("Small symb", 0); // show on display
    // LCD_Goto(0, 3);
    // LCD_Printf("BIGGER_SYMB", 1);
    // _delay_ms(3000);

    // template on the screen
    LCD_DrawImage(0); // CLEAR
    LCD_Goto(0, 1);
    LCD_Printf("WiFi TEXT:", 1);
    LCD_Goto(0, 5);
    LCD_Printf("E", 1);
    LCD_Goto(0, 6);
    LCD_Printf("N", 1);
    LCD_Goto(0, 7);
    LCD_Printf("C", 1);
    LCD_Goto(116, 5);
    LCD_Printf("E", 1);
    LCD_Goto(116, 6);
    LCD_Printf("N", 1);
    LCD_Goto(116, 7);
    LCD_Printf("C", 1);
    LCD_Goto(0, 3);
    LCD_Printf("Nothing recieved", 0);

    sei();

    while(1) {
        // encoder counter reveal
        LCD_Goto(27, 4);
        counter = enc_counter;
        IntToStr(str, (int16_t)counter);
        if(counter < 100)
            LCD_BigNum(' ');
        if(counter < 10)
            LCD_BigNum(' ');
        LCD_Printf(str, 2);

        hue_to_rgb(counter << 3);
        if(led_on > 0) {
            Led(GLOBAL_GRB[1], GLOBAL_GRB[0], GLOBAL_GRB[2]);
        } else {
            Led(GLOBAL_GRB[1] >> 5, GLOBAL_GRB[0] >> 5, GLOBAL_GRB[2] >> 5);
        }
    }
}
