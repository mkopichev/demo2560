#include "avr/io.h"

void WiFi_main_processing(void);
void WiFi_send_command(uint8_t id, uint8_t Connection, uint16_t Send_N);

void Edit_HTML(void);
void WiFi_processing_clear(void);
inline void WiFi_get_answer(void);
void WiFi_setup(void);

uint8_t WiFi_Current_Connection;//connection number 0-4
//uint16_t WiFi_Send_N;//symbols number 2048 max
//uint8_t WiFi_Error=0;
uint8_t WiFi_what_to_read=0,WiFi_buf[5],WiFi_answer_ID=0,WiFi_symbol_counter=0;
uint8_t WiFi_Busy=0;
uint8_t WiFi_reset_no_activity_counter=16;//if no activity - reset and reinit wifi (initial 8s)
uint8_t HTML_info_N=0;

//what each connection wants 0-nothing 
//1-request known proceed to process value
//.....
//100-request unknown just close the connection
uint8_t WiFi_Connection_Request[5]={0,0,0,0,0};
uint8_t WiFi_Request_Value[5][30];
uint8_t WiFi_Connection_CommandID[5];
uint16_t WiFi_reset_flag=0;

//now the code is in RAM, if code is more than 4k turn it into const(flash) ram is not enough !!!!!!!!!!!!!!!!!!!
uint8_t HTML_main_code[246+63+17+17+16+60+113+75+1]=
//246
"<head><link rel='icon'href='data:image/bmp;base64,Qk1+AAAAAAAAAD4AAAAoAAAAEAAAABAAAAABAAEAAAAAAEAAAAB0EgAAdBIAAAAAAAAAAAAAAAAAAP///wAP/AAAP/4AAGAGAADKqwAAiqkAAMADAAD//wAA/j8AAMcjAAC7XQAAq1UAALpdAADH4wAAP/4AAAqqAAAAAAAA'type='image/x-png'/></head>"
//63
"<style>h1,h2,h3,input,i,select{font-size:40px}h1{color:#0000f0}"
//17   HTML_main_code[246+63+10]= er color[0] (00a000)
"h2{color:#a0a000}"
//17   HTML_main_code[246+63+17+10]= color[0]
"h3{color:#f0f000}"
//16
"i{color:#990073}"
//60
"input,select{color:#009999}</style><h1>2560MODULE ���� </h1>"
//113
"<form><i>Text to OLED display:</i><input name='T'size='20'maxlength='20'><input type='submit'value='SEND'></form>"
//75
"<form><i>RGB LED ON/OFF:</i><input type='submit'name='L'value='LED'></form>";

uint8_t HTML_info_code[7][34+1]=
{
"<h3>TEXT SHOULD GO TO OLED   </h3>",
"<h3>UNBELIEVABLE  SUCCESS !!!</h3>",
"<h3> YET ANOTHER SUCCESS!    </h3>",
"<h3>WOW! SO MUCH SUCCESS...  </h3>",
"<h3>REALLY? AREN'T YOU TIRED?</h3>",
"<h3>YOU ARE THE LED RULER!!! </h3>",
"<h3>STILL FUN, HA?           </h3>"
};
