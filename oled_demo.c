#include "include/oled_demo.h"

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

    Ws2812bInit();     // set rgb pin
    EncoderIncrInit(); // set encoder pins

    // TWI initialization
    // Mode: TWI Master
    // Bit Rate: 400 kHz
    I2cInit();

    _delay_ms(100);
    LCD_init();
    _delay_ms(1);

    // dog animation
    for(tmp = 0; tmp <= 9; tmp++) {
        LCD_DrawImage(dog2_0);
        LCD_DrawImage(dog2_1);
        LCD_DrawImage(dog2_2);
        LCD_DrawImage(dog2_3);
        LCD_DrawImage(dog2_4);
        LCD_DrawImage(dog2_5);
        LCD_DrawImage(dog2_6);
        LCD_DrawImage(dog2_7);
        LCD_DrawImage(dog2_8);
        LCD_DrawImage(dog2_9);
        LCD_DrawImage(dog2_10);
        LCD_DrawImage(dog2_11);
        LCD_DrawImage(dog2_12);
    }
    LCD_DrawImage(0); // CLEAR

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
    LCD_Goto(118, 5);
    LCD_Printf("E", 1);
    LCD_Goto(118, 6);
    LCD_Printf("N", 1);
    LCD_Goto(118, 7);
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

        HueToRgb(counter << 3);
        if(led_on > 0) {
            Led(GLOBAL_GRB[1], GLOBAL_GRB[0], GLOBAL_GRB[2]);
        } else {
            Led(GLOBAL_GRB[1] >> 5, GLOBAL_GRB[0] >> 5, GLOBAL_GRB[2] >> 5);
        }
    }
}
