/*******************************************************
OLED дисплей на SSD1306 I2C демо для CVAVR atmega2560
ssd1306_lib_new.c - файл библиотеки OLED
fonts_pictures_oled.c - файл со шрифтами и картинками

Экран подключается по аппаратному TWI.

#define I2C_hard- если аппаратный, если програмный - закоментить

Аппаратный I2C быстрее в 2 раза на той-же скорости (100К)  но почему то больше памяти занимает(из за библиотеки).
Аппаратный работает и на 400K, програмный 100К максимум.
 
для работы с другим контроллером или портами, включить и настроить I2C в своиствах проекта (если програмный)
при нехватки флэш памяти выводить меньше картинок, закоментить анимацию собаки например.

Библиотеку я подправил для ускорения вывода картинок, использование аппаратного АйТуСи и исправил всякий трэш.  

Версия Мишаня эдишн:)   
*******************************************************/

#include "include/oled_demo.h"
#include "include/ssd1306_lib_new.h"

void IntToStr(uint8_t* st, int16_t n)//16 bit int to str uint8_t ?????????????????
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

void demo_oled(void)
{
uint8_t tmp,counter=0;
uint8_t str[20];
// // Crystal Oscillator division factor: 1
// #pragma optsize-
CLKPR=(1<<CLKPCE);
CLKPR=(0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
// #ifdef _OPTIMIZE_SIZE_
// #pragma optsize+
// #endif

//I2C �����
DDRD &= ~(1<<0);//SCL
DDRD &= ~(1<<1);//SDA
PORTD |= (1<<0);//pull-up
PORTD |= (1<<1);//pull-up

 // TWI initialization
 // Mode: TWI Master
 // Bit Rate: 100 kHz
 I2cInit();
 
 _delay_ms(100);
 LCD_init();
 _delay_ms(1);  
 sei();
 
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

   PORTH|=(1<<5);

   LCD_Goto(0,1); 
   LCD_Printf("������� ������ �����",0);  //  ����� �� �������
   LCD_Goto(0,3); 
   LCD_Printf("��������!",1);  //  ����� �� �������
 while (1)                                  
  {                                        
   counter++;
   LCD_Goto(5,4);
   IntToStr(str,counter); 
   if(counter<100)LCD_BigNum(' ');
   if(counter<10)LCD_BigNum(' ');        
   LCD_Printf(str,2);  //  ����� �� �������      
  }
}
