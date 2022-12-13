#include "include/oled_demo.h"

#define OLED_DEMO 1
#define WIFI_DEMO 2
#define COMPLEX_DEMO 3

#define PROGRAM_ROUTINE OLED_DEMO

int main(void){
  
  DDRH|=(1<<5);

  #if(PROGRAM_ROUTINE == OLED_DEMO)
    demo_oled();
  #elif(PROGRAM_ROUTINE == WIFI_DEMO)
    demo_WiFi();
  #elif(PROGRAM_ROUTINE == COMPLEX_DEMO)
    my2560_demo();
  #endif

  return 0;
}