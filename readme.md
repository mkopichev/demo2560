
# DemoProg for atmega2560 demo module

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

## Files: 

ssd1306_lib_new.c - ssd1306 OLED library

>>Initially is written for CVAVR
>>22.12.2015 Vinichenko A.V.
>>29.07.2020 changed by Putov A.V.
>>16.12.2022 changed for VS Code by Belskiy G.V.

fonts_pictures_oled.c - fonts and pictures

Display is using harware TWI.

Harware TWI works on 400k Hz speed.

To work with another controller or ports, enable and configure TWI in
your own projects (if programmatic). If less of flash output less pictures,
comment out the animation of the dog for instance.

Library is finalized for higher excecution speed.

Mishanja edition version :)
