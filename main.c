/*
Demo code for checking the operation of atmega2560 module with SSD1306, ESP-12 and JDY-40.
CVAVR original, VS Code changed (2022, december).
Parameters: 11.0592 MHz, FUSE all cleared (1), except SPIEN - it is set (0).

Encoder angle reveals on the screen.
RGB LED changes color according to encoder count.
Push encoder and RGB LED will change its britness.
Program organises Soft AP, connection to which let user to send english text on the screen
and change the britness of WS2812B.

For connection, connect to WiFi network 2560DEMO (on the phone turn off data transmittion),
goto browser and type 192.168.0.100, after that simple HTML should appear.

Code send letter 'T' to JDY-40 via UART 9600 baud. USB-UART needed.

Turn on voltage regulator!
*/

#include "include/oled_demo.h"

#define OLED_DEMO    1
#define WIFI_DEMO    2
#define COMPLEX_DEMO 3

#define PROGRAM_ROUTINE OLED_DEMO

int main(void) {
#if(PROGRAM_ROUTINE == OLED_DEMO)
    demo_oled();
#elif(PROGRAM_ROUTINE == WIFI_DEMO)
    demo_WiFi();
#elif(PROGRAM_ROUTINE == COMPLEX_DEMO)
    my2560_demo();
#endif

    return 0;
}