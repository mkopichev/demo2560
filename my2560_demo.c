/*******************************************************
���� ��������� ��� �������� ����������������� ������ atmega2560 c OLED, ESP-12 � jdy-40 
CVAVR 
11.0592 MHz  Fuse bits ��� �� �����������(1) (����� SPIEN(0)(� CodeVision SPIEN ����� , ��� � �������))

��������� ������� �� ���� ������� ��������� ��������,
�������� �������� ����� ������ ���� RGB LED. 
������� �� ������� ����-������ RGB LED. 
����� ��������� ���������� Soft AP ��� ����������� � ������� 
����� �������� �����(english) �� ������� � ����-������ RGB LED.
�����.
����� ������������ ���� ����������� � WiFi 2560DEMO (���� � ������ �� ��������� �������� ������)
� ����� �� �������� �� ����� 192.168.0.100 ������ �������� ����������� HTML ��������.

����� ��������� ��������� ������� ����� 'T' � jdy-40(����������� ��� 9600),
����� ��������� �� ���������� ���� ������ JDY-40 ������������ � UART-USB...
*******************************************************/

//�������� ������!!!!!!!!!!!!!!!!!!!!!

#include "include/utils.h"

#include "include/fonts_pictures_oled.h"
#include "include/ssd1306_lib_new.h"
//#include <demo_WIFI.c> included later after variables.......

#define RGBPIN PORTA&(1<<7) //rgb led

#define WIFIOFF DDRD&(1<<4) //open drain 1-off 0-on 
#define WIFIRES DDRD&(1<<5) //open drain 1-res 0-norm working 
#define WiFi_reset_time 2000 //1000seconds after any activity
 
#define WiFi_ON
//#define WiFi_UART0_OUT 
 
void uCsetup(void);
void UART_setup(uint8_t UART_N,uint8_t baud_N);//baud_N=0 - 9600 baud_N=1 - 115200
void putuint8_t0(uint8_t c);//jdy-40
void putuint8_t1(uint8_t c);//Wifi
void putstr(uint8_t com_n,const uint8_t* S);
void IntToStr(uint8_t* st,int16_t n);//16 bit int16_tto str uint8_t
//OLED and encoder interrupts
// interrupt [PC_INT0] void pin_change_isr0(void);
ISR(PCINT0_vect);
// interrupt [PC_INT1] void pin_change_isr1(void);
ISR(PCINT1_vect);
void Led(uint8_t R, uint8_t G, uint8_t B);
void hue_to_rgb(uint8_t hue);
void T3_delay(uint8_t d);//1-255 * 23.7ms
void T3_timeout(uint8_t d);//1-255 * 23.7ms
//Encoder
uint8_t PB4_B=1,PB5_B=1,ENC_counter=0;
//Encoder

uint8_t T3_ready=0;

uint8_t GLOBAL_GRB[3]={0,0,0};
uint8_t OLED_text[30]={"���� �� ��������..."};
uint8_t Ledon=0;

#include "include/demo_WiFi.h"

// interrupt [USART0_RXC] void usart0_rx_isr(void)
ISR(USART0_RXC_vect)
{
uint8_t status,data;
status=UCSR0A;
data=UDR0;
}

// Timer1 overflow interrupt service routine 0.5s
// interrupt [TIM1_OVF] void timer1_ovf_isr(void)
ISR(TIM1_OVF_vect)
{
// Reinitialize Timer1 value
TCNT1H=0xABA0 >> 8;
TCNT1L=0xABA0 & 0xff;

if(WiFi_reset_no_activity_counter>0) WiFi_reset_no_activity_counter--;

}


void main(void)
{
uint8_t tmp,counter=0;
uint8_t str[20];

uCsetup(); 
WIFIOFF=0;
 
 _delay_ms(100);
 LCD_init();
 _delay_ms(1);
 UART_setup(0,0);
 sei()
 
 for(tmp=0;tmp<3;tmp++)//�������� ������
  {
    LCD_DrawImage(dog0);  
    LCD_DrawImage(dog1);
    LCD_DrawImage(dog2);
    LCD_DrawImage(dog3);
    LCD_DrawImage(dog4);
    LCD_DrawImage(dog5);
    LCD_DrawImage(dog6);        
  } 
  
   LCD_Commmand(COMAND, SSD1306_INVERTDISPLAY); 
   LCD_DrawImage(mishaniya);
   _delay_ms(5000);
   LCD_Commmand(COMAND, SSD1306_NORMALDISPLAY);
   LCD_DrawImage(0);//CLEAR
      
   LCD_Goto(0,1);                                    
   LCD_Printf("WiFi �����:",1);  //  ����� �� ������� 
   LCD_Goto(0,5);                                    
   LCD_Printf("E",1);  //  ����� �� �������
   LCD_Goto(0,6);                                    
   LCD_Printf("N",1);  //  ����� �� �������
   LCD_Goto(0,7);                                    
   LCD_Printf("C",1);  //  ����� �� �������
   LCD_Goto(116,5);                                    
   LCD_Printf("E",1);  //  ����� �� �������
   LCD_Goto(116,6);                                    
   LCD_Printf("N",1);  //  ����� �� �������
   LCD_Goto(116,7);                                    
   LCD_Printf("C",1);  //  ����� �� �������
   LCD_Goto(0,3);
   LCD_Printf(OLED_text,0);  //  ����� �� �������  
    #ifdef WiFi_ON
   WiFi_setup();//!!!!!!!!!!!!!!!!!!! 
    #endif
 while (1)                                  
  {                                           
   LCD_Goto(27,4); 
   counter=ENC_counter;
   IntToStr(str,counter); 
   if(counter<100)LCD_BigNum(' ');
   if(counter<10)LCD_BigNum(' ');        
   LCD_Printf(str,2);  //  ����� �� ������� 
   putuint8_t0('A');//Jdy-40 out
    
   hue_to_rgb(counter<<3);
   if(Ledon>0) 
    {Led(GLOBAL_GRB[1], GLOBAL_GRB[0], GLOBAL_GRB[2]);}
   else
    {Led(GLOBAL_GRB[1]>>5, GLOBAL_GRB[0]>>5, GLOBAL_GRB[2]>>5);}
    
   #ifdef WiFi_ON  
   WiFi_main_processing();//!!!!!!!!!!!!!
   LCD_Goto(0,3); 
   LCD_Printf(OLED_text,0);  //  ����� �� �������
   
   
//        if(WiFi_reset_no_activity_counter==0)
//        {
//         WiFi_reset_no_activity_counter=WiFi_reset_time;
//         WiFi_processing_clear();
//         WIFIRES=1;// res
//         T3_delay(4);//0.1S
//         WIFIRES=0;//NO res
//        }
//        else
//        {
//         if(WiFi_reset_flag==1)//reset detected
//         {
//          WiFi_reset_flag=0;
//          WiFi_processing_clear();
//          WiFi_setup();
//         }
//         else
//         {WiFi_main_processing();}//WiFi processing
//        }                                                                                                        
        #endif   
  }
}



void uCsetup(void)
{
 // Crystal Oscillator division factor: 1
    #pragma optsize-
    CLKPR=(1<<CLKPCE);
    CLKPR=(0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
    #ifdef _OPTIMIZE_SIZE_
    #pragma optsize+
    #endif

    //I2C �����
    DDRD.0=0;//SCL
    DDRD.1=0;//SDA
    PORTD.0=1;//pull-up
    PORTD.1=1;//pull-up

    DDRF.0=1;//mic5219(wifi and jdy-40) enable               
    PORTF.0=1;//mic5219(wifi and jdy-40) enable
    DDRA.7=1;//led


    // PCINT4 interrupt: On
    // PCINT5 interrupt: On
    // PCINT12 interrupt: On
    PCMSK0=(0<<PCINT7) | (0<<PCINT6) | (1<<PCINT5) | (1<<PCINT4) | (0<<PCINT3) | (0<<PCINT2) | (0<<PCINT1) | (0<<PCINT0);
    PCMSK1=(0<<PCINT15) | (0<<PCINT14) | (0<<PCINT13) | (1<<PCINT12) | (0<<PCINT11) | (0<<PCINT10) | (0<<PCINT9) | (0<<PCINT8);
    PCICR=(0<<PCIE2) | (1<<PCIE1) | (1<<PCIE0);

    
    // Timer/Counter 3 initialization
    // Clock value: 10,800 kHz
    // Mode: Normal top=0xFFFF
    // Timer3 Overflow Interrupt: Off, (turned On when the function T3_delay() is called)!!!!!!!!!!!!!!!!!!!  
    TCCR3A=(0<<COM3A1) | (0<<COM3A0) | (0<<COM3B1) | (0<<COM3B0) | (0<<COM3C1) | (0<<COM3C0) | (0<<WGM31) | (0<<WGM30);
    TCCR3B=0;
    TCCR3B=0;
    TCNT3H=0x00;
    TCNT3L=0x00;  
    
    // Timer/Counter 3 Interrupt(s) initialization
    TIMSK3=(0<<ICIE3) | (0<<OCIE3C) | (0<<OCIE3B) | (0<<OCIE3A) | (1<<TOIE3);
     
    // Timer/Counter 1 initialization
    // Clock value: 43,200 kHz
    // Mode: Normal top=0xFFFF
    // Timer Period: 0,5 s
    // Timer1 Overflow Interrupt: On
    TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (1<<CS12) | (0<<CS11) | (0<<CS10);
    TCNT1H=0xAB;
    TCNT1L=0xA0;

    // Timer/Counter 1 Interrupt(s) initialization
    TIMSK1=(0<<ICIE1) | (0<<OCIE1C) | (0<<OCIE1B) | (0<<OCIE1A) | (1<<TOIE1);    

     // TWI initialization
     // Mode: TWI Master
     // Bit Rate: 100 kHz
     twi_master_init(100);
}

void UART_setup(uint8_t UART_N,uint8_t baud_N)//baud_N=0 - 9600 baud_N=1 - 115200 
{
  switch(UART_N)
  {  
   case 0:
    //USART0 Baud Rate: 115200
    UCSR0A=(0<<RXC0) | (0<<TXC0) | (0<<UDRE0) | (0<<FE0) | (0<<DOR0) | (0<<UPE0) | (0<<U2X0) | (0<<MPCM0);
    UCSR0B=(1<<RXCIE0) | (0<<TXCIE0) | (0<<UDRIE0) | (1<<RXEN0) | (1<<TXEN0) | (0<<UCSZ02) | (0<<RXB80) | (0<<TXB80);
    UCSR0C=(0<<UMSEL01) | (0<<UMSEL00) | (0<<UPM01) | (0<<UPM00) | (0<<USBS0) | (1<<UCSZ01) | (1<<UCSZ00) | (0<<UCPOL0);
    UBRR0H=0x00;
    if(baud_N==0) UBRR0L=0x47;
    else UBRR0L=0x05;
   break;          
   case 1:
    //USART1 Baud Rate: 115200
    UCSR1A=(0<<RXC1) | (0<<TXC1) | (0<<UDRE1) | (0<<FE1) | (0<<DOR1) | (0<<UPE1) | (0<<U2X1) | (0<<MPCM1);
    UCSR1B=(1<<RXCIE1) | (0<<TXCIE1) | (0<<UDRIE1) | (1<<RXEN1) | (1<<TXEN1) | (0<<UCSZ12) | (0<<RXB81) | (0<<TXB81);
    UCSR1C=(0<<UMSEL11) | (0<<UMSEL10) | (0<<UPM11) | (0<<UPM10) | (0<<USBS1) | (1<<UCSZ11) | (1<<UCSZ10) | (0<<UCPOL1);
    UBRR1H=0x00;
    if(baud_N==0) UBRR1L=0x47;
    else UBRR1L=0x05;
   break;         
   case 2:
    //USART2 Baud Rate: 115200
    UCSR2A=(0<<RXC2) | (0<<TXC2) | (0<<UDRE2) | (0<<FE2) | (0<<DOR2) | (0<<UPE2) | (0<<U2X2) | (0<<MPCM2);
    UCSR2B=(1<<RXCIE2) | (0<<TXCIE2) | (0<<UDRIE2) | (1<<RXEN2) | (1<<TXEN2) | (0<<UCSZ22) | (0<<RXB82) | (0<<TXB82);
    UCSR2C=(0<<UMSEL21) | (0<<UMSEL20) | (0<<UPM21) | (0<<UPM20) | (0<<USBS2) | (1<<UCSZ21) | (1<<UCSZ20) | (0<<UCPOL2);
    UBRR2H=0x00;
    if(baud_N==0) UBRR2L=0x47;
    else UBRR2L=0x05;
   break;  
   case 3: 
    //USART3 Baud Rate: 115200
    UCSR3A=(0<<RXC3) | (0<<TXC3) | (0<<UDRE3) | (0<<FE3) | (0<<DOR3) | (0<<UPE3) | (0<<U2X3) | (0<<MPCM3);
    UCSR3B=(1<<RXCIE3) | (0<<TXCIE3) | (0<<UDRIE3) | (1<<RXEN3) | (1<<TXEN3) | (0<<UCSZ32) | (0<<RXB83) | (0<<TXB83);
    UCSR3C=(0<<UMSEL31) | (0<<UMSEL30) | (0<<UPM31) | (0<<UPM30) | (0<<USBS3) | (1<<UCSZ31) | (1<<UCSZ30) | (0<<UCPOL3);
    UBRR3H=0x00;
    if(baud_N==0) UBRR3L=0x47;
    else UBRR3L=0x05; 
  } 
}


void putuint8_t0(uint8_t c)
{
while ( !( UCSR0A & (1<<5)) ){} //UDRE0
 UDR0=c;
}


void putuint8_t1(uint8_t c)//Wifi
{
while ( !( UCSR1A & (1<<5)) ){}
 UDR1=c;          
 //UDR0=c;//shitty send test
}

void putstr(uint8_t com_n,const uint8_t* S)
{
 uint16_t tmp=0;
 while(S[tmp]!=0)
 {
  switch(com_n)
  {
   case 0:
   putuint8_t0(S[tmp]);
   break;
   case 1:    
   putuint8_t1(S[tmp]);
   break;
 //  case 2:          
//   putuint8_t2(S[tmp]);
//   break;
//   case 3:          
 //  putuint8_t3(S[tmp]);   
  } 
  tmp++;
 }
}

void IntToStr(uint8_t* st,int16_t n)//16 bit int16_tto str uint8_t 
{
 uint8_t tmp2;
 uint16_t tmp=10000,tmpn,ind=0;
 if(n<0)
 {n=-n;  
  st[ind]='-';
  ind++;}
            
 tmpn=n;
 
 if(n==0)  
 {
  st[0]=0+48;
  st[1]=0;
  return;
 }  
        
 while(tmp>0) 
 {       
  if(tmp<=n)
  {     
   tmp2=tmpn/tmp;
   st[ind]=tmp2+48;
   tmpn=tmpn-tmp2*tmp;    
   ind++;
  } 
  tmp=tmp/10;
 }        
 st[ind]=0;
}

// Pin change 0-7 interrupt service routine
// interrupt [PC_INT0] void pin_change_isr0(void)
ISR(PCINT0_vect)
{
// Place your code here
if(PINB.4==0)
 { 
  if(PINB.5==1&&PB5_B==1){ENC_counter--;} 
 }
 else
 {
  if(PINB.5==0&&PB4_B==1){ENC_counter++;}   
 }
 PB4_B=PINB.4; 
 PB5_B=PINB.5;
}

// Pin change 8-15 interrupt service routine
// interrupt [PC_INT1] void pin_change_isr1(void)
ISR(PCINT1_vect)
{
 if((PINJ&(0b00001000))>0)
 { 
  if(Ledon>0)
   Ledon=0;
  else
   Ledon=1; 
 }
}

 
void Led(uint8_t R, uint8_t G, uint8_t B)//��� RGB ����������  11.0592 MHz
{
    uint8_t i;  
    for(i = 0b10000000; i >= 0b00000001; i = i >> 1)//Green
    {
        if((G & i) > 0)
        {RGBPIN=1;//should be 0.9us
         _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
         RGBPIN=0;}//should be 0.35us but ??1.35us also works ok// now it's dick knows  
        else
        {RGBPIN=1; //should be 0.35us but 0.4 
         _NOP();
         RGBPIN=0;} //should be 0.9us but 1.35us also works ok      
    }      
    for(i = 0b10000000; i >= 0b00000001; i = i >> 1)//Red
    {
        if((R & i) > 0)
        {RGBPIN=1;//should be 0.9us
         _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
         RGBPIN=0;}//should be 0.35us but 1.35us also works ok
            
        else
        {RGBPIN=1; //should be 0.35us but 0.4 
         _NOP();
         RGBPIN=0;} //should be 0.9us but 1.35us also works ok                    
    }      
    for(i = 0b10000000; i >= 0b00000001; i = i >> 1)//Blue
    {
        if((B & i) > 0)
        {RGBPIN=1;//should be 0.9us
         _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
         RGBPIN=0;}//should be 0.35us but 1.35us also works ok             
        else
        {RGBPIN=1; //should be 0.35us but 0.4 
         _NOP();
         RGBPIN=0;} //should be 0.9us but 1.35us also works ok                    
    }        
}

void hue_to_rgb(uint8_t hue)
{
 if(hue<=85)
 {
  GLOBAL_GRB[2]=(85-hue)*3;        // GLOBAL_GRB[0]RB[2]
  GLOBAL_GRB[1]=hue*3;
  GLOBAL_GRB[0]=0;
  return;
 }
  if(hue>170)
 {
  GLOBAL_GRB[2]=(hue-171)*3;  
  GLOBAL_GRB[1]=0;
  GLOBAL_GRB[0]=(255-hue)*3;
  return;
 }
 else  
 {
  GLOBAL_GRB[2]=0;  
  GLOBAL_GRB[1]=(171-hue)*3;
  GLOBAL_GRB[0]=(hue-86)*3;
  return;
 }                    
}

//Timer3 DELAY TIMEOUT-------------------------------------------------------------
// Timer3 overflow interrupt
//General purpose timer (delays timeouts etc)
//max P=6s (10800Hz  65535)
ISR(TIMER3_OVF_vect)
{
 TCCR3B=0;//t3 off
 T3_ready=1;   
}

void T3_delay(uint8_t d)//1-255 * 23.7ms
{
 T3_ready=0;
 TCNT3H=(255-d)+1;
 TCNT3L=0;
 TCCR3B=5;//T3 on 10800Hz
 while(T3_ready==0){};
}


//start timeout (do smth until overflow interrupt set T3_ready=1)
void T3_timeout(uint8_t d)//1-255 * 23.7ms
{
 T3_ready=0;
 TCNT3H=(255-d)+1;
 TCNT3L=0;
 TCCR3B=5;//T3 on 10800Hz
}
//Timer3 DELAY-------------------------------------------------------------