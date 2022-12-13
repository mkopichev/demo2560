#include "include/demo_WiFi.h"

//WiFi ������� ��� ���� ��������� 2560������
//ESP-12f ����������� �������� �������� �� ��������� 115200

//change parameters and colors in HTML_main_code according to the actual  
void Edit_HTML(void)
{
 //set info text color
 HTML_main_code[246+63+17+10]='0';//R
 HTML_main_code[246+63+17+10+2]='0';//G
 HTML_main_code[246+63+17+10+4]='0';//B
 switch(HTML_info_N)
 {
  case 0:  
  HTML_main_code[246+63+17+10+4]='f';
  break;  
  case 5:
  case 1: 
  HTML_main_code[246+63+17+10+2]='f';
  break; 
  case 6:
  case 2: 
  HTML_main_code[246+63+17+10]='f';
  break;
  case 3: 
  HTML_main_code[246+63+17+10]='f';  
  HTML_main_code[246+63+17+10+2]='f';
  break;
  case 4: 
  HTML_main_code[246+63+17+10+2]='f';
  HTML_main_code[246+63+17+10+4]='f';
  break;
 }
}


void WiFi_processing_clear(void)
{
 uint8_t tmp;
 WiFi_answer_ID=0;
 WiFi_what_to_read=1;
 for(tmp=0;tmp<5;tmp++) WiFi_Connection_Request[tmp]=0; 
}

void WiFi_main_processing(void)
{
 uint8_t tmp,tmp2,tmp_flag;
        for(tmp=0;tmp<5;tmp++)//if there is any request
        {
         if(WiFi_Connection_Request[tmp]>0)
         {  
          WiFi_reset_no_activity_counter=WiFi_reset_time;                           
          if(WiFi_Connection_CommandID[tmp]>0)//known command (should send data)
          { 
           T3_timeout(42);//42=1s 
           if(WiFi_Connection_CommandID[tmp]==6)//text to oled
           {
            tmp_flag=0;
            for(tmp2=0;tmp2<20;tmp2++)
            {
             if(WiFi_Request_Value[tmp][tmp2]==0)tmp_flag=1;//if shorter than 20 letters put ' '   
             if(WiFi_Request_Value[tmp][tmp2]=='+')WiFi_Request_Value[tmp][tmp2]=' ';
                    
             if(tmp_flag==0)
              OLED_text[tmp2]=WiFi_Request_Value[tmp][tmp2];
             else
              OLED_text[tmp2]=' '; 
            }                                             
            OLED_text[20]=0;//20 letters anyway
            HTML_info_N++;
             if(HTML_info_N>4) HTML_info_N=1; 
           } 
           if(WiFi_Connection_CommandID[tmp]==7)//led on off
           {
            if(Ledon==0)Ledon=1;
            else Ledon=0;
            if(HTML_info_N==5)HTML_info_N=6;
            else HTML_info_N=5; 
           }
           Edit_HTML(); 
           while(WiFi_Busy>0) {if(T3_ready==1) break;};
           WiFi_send_command(8,tmp,(246+63+17+17+16+60+113+75) + 34);//send the main code + text
           T3_timeout(21);//42=1s
           while(WiFi_answer_ID!=3){if(T3_ready==1) break;};//invitation
           putstr(1,HTML_main_code);
           putstr(1,HTML_info_code[HTML_info_N]);             
           T3_timeout(21);//42=1s
           while(WiFi_answer_ID!=1){if(T3_ready==1) break;};//ok
           WiFi_send_command(7,tmp,0);//close the connection
           
          }
          WiFi_Connection_Request[tmp]=0;
          break; //only one request at a time  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
         }
        }
}



//G_answer_ID>0 there is an answer 0-no match
//1- OK
//2- ERROR
//3- >
//4- +IPD, 
//5- GET
//6- ?T=..... HTTP/1.1   //TEXT

//30- HTTP/1.1   //http only    
//50- (13)(10)(13)(10) end of useless info    to wait or not to wait?????????????
//60- ready(13)(10) after reset to reinitialise
inline void WiFi_get_answer(void)
{
 WiFi_answer_ID=0;
 
 if(WiFi_buf[2]=='O'&&WiFi_buf[3]=='K'&&WiFi_buf[4]==13)
  {WiFi_answer_ID=1; //OK
   return;}
  
 if(WiFi_buf[0]=='E'&&WiFi_buf[1]=='R'&&WiFi_buf[2]=='R')
  {WiFi_answer_ID=2; //ERROR
   return;}
 
 if(WiFi_buf[2]==10&&WiFi_buf[3]=='>'&&WiFi_buf[4]==32)
  {WiFi_answer_ID=3; //> _//welcome to send sms
   return;}
                                                         
 if(WiFi_buf[0]=='I'&&WiFi_buf[1]=='P'&&WiFi_buf[2]=='D'&&WiFi_buf[3]==',')
  {WiFi_answer_ID=4; //+IPD,   request start, WiFi_buf[4]=0...3 digit is Current_Connection
   return;}
   
 if(WiFi_buf[0]=='G'&&WiFi_buf[1]=='E'&&WiFi_buf[2]=='T'&&WiFi_buf[3]==' '&&WiFi_buf[4]=='/')
  {WiFi_answer_ID=5; //GET /    next will be what they want us to send
   return;}                
  
 if(WiFi_buf[2]=='?'&&WiFi_buf[4]=='=')
  {                 
   if(WiFi_buf[3]=='T'){WiFi_answer_ID=6; return;}//?T=  text      
    if(WiFi_buf[3]=='L'){WiFi_answer_ID=7; return;}//?L=  led on off
   WiFi_answer_ID=30;//if unknown send just HTTP
  }
   
 if(WiFi_buf[2]=='/'&&WiFi_buf[3]==' '&&WiFi_buf[4]=='H')//http only 
 {WiFi_answer_ID=30; return;}         
   
 if(WiFi_buf[2]==13&&WiFi_buf[3]==10&&WiFi_buf[4]==13)//end of useless info 
 {WiFi_answer_ID=50;
  return;}
   
 if(WiFi_buf[0]=='a'&&WiFi_buf[1]=='d'&&WiFi_buf[2]=='y'&&WiFi_buf[3]==13&&WiFi_buf[4]==10)//after reset 
 {
  WiFi_reset_flag=1;
 }               
}


//USART1 Receiver interrupt
//ESP-12F(ESP8266) module serial 
//Read sms, responses etc
//WiFi_what_to_read=
//1- any answer .............
// interrupt [USART1_RXC] void usart1_rx_isr(void)
ISR(USART1_RXC_vect)
{
 uint8_t data;
 data=UDR1; 
  
 
 #ifdef WiFi_UART0_OUT
 UDR0=data;//shitty send test
 #endif
 
 #ifdef WiFi_ON           
 WiFi_buf[0]=WiFi_buf[1]; WiFi_buf[1]=WiFi_buf[2]; WiFi_buf[2]=WiFi_buf[3]; WiFi_buf[3]=WiFi_buf[4]; WiFi_buf[4]=data;//buffer shift anyway
 
 switch(WiFi_what_to_read)
 {
  case 0: //nothing
  break;
  case 1: //limitless wait for the answer and switch 
  //then set answerID         
   WiFi_get_answer();//if WiFi_buf contains an answer - set the answer_ID 
   if(WiFi_answer_ID>0)
   {               
     switch(WiFi_answer_ID)
     {
      case 4: //incoming +IPD, request start, WiFi_buf[4]=0...4 digit is Current_Connection
      // WiFi_Test++;   
       WiFi_Busy=1;    
       WiFi_Current_Connection=WiFi_buf[4]-'0'; //0 -4
       if(WiFi_Current_Connection>4) break; //search no more
       WiFi_symbol_counter=0;//?????
       WiFi_what_to_read=2; //next step 
      return;
     }
   }  
  break;
  case 2: //waiting for get
   if(WiFi_symbol_counter<13)//limit 13 symb
   {   
    WiFi_symbol_counter++;
    WiFi_get_answer();
    if(WiFi_answer_ID==5)
    { 
     //WiFi_Test++;
     WiFi_what_to_read=3;//next step      
     WiFi_symbol_counter=0;
     //WiFi_Connection_Request[WiFi_Current_Connection]=1;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
     WiFi_Connection_CommandID[WiFi_Current_Connection]=0;//unknown for now
    }                   
    return;//next step
   } 
   WiFi_what_to_read=5;//next step                          
  return;
  case 3: //waiting for request
  if(WiFi_symbol_counter<6)//limit 6 symb
   {   
    WiFi_symbol_counter++;
    WiFi_get_answer();
    if(WiFi_answer_ID==30)//html or button
    {      
     //do smth in main
     WiFi_Connection_CommandID[WiFi_Current_Connection]=WiFi_answer_ID;
     WiFi_Connection_Request[WiFi_Current_Connection]=1;
     WiFi_what_to_read=5;//next step(getting value)
     return;    
    }                         
    if(WiFi_answer_ID>=6&&WiFi_answer_ID<=12)//parameters set 
    {   
     WiFi_what_to_read=4;//next step(getting value)
     WiFi_symbol_counter=0;                        
     WiFi_Connection_CommandID[WiFi_Current_Connection]=WiFi_answer_ID;
    }
    return;//continue
   } 
  WiFi_Connection_Request[WiFi_Current_Connection]=1;//unknown at all but should be closed in main                   
  break;
  case 4: //waiting for request value
   if(WiFi_symbol_counter<30)//limit 30 symb
   {                       
    if(data==' ')//' '
    {
     WiFi_Request_Value[WiFi_Current_Connection][WiFi_symbol_counter]=0;//close uint8_t string
     WiFi_Connection_Request[WiFi_Current_Connection]=1;
     //set par and do anything in main
     WiFi_what_to_read=5;//next step(getting value)
     return;  
    }
    else
    { 
     WiFi_Request_Value[WiFi_Current_Connection][WiFi_symbol_counter]=data;
    }        
    WiFi_symbol_counter++;
    return;//further
   }  
   WiFi_Connection_Request[WiFi_Current_Connection]=1;
   WiFi_Request_Value[WiFi_Current_Connection][WiFi_symbol_counter]=0;
   WiFi_what_to_read=5;//next step
   return;
  case 5: //waiting for the end of semiuseless crap
   WiFi_get_answer();//if WiFi_buf contains an answer then set the answer_ID 
   if(WiFi_answer_ID==50)
   {         
     WiFi_Busy=0;
     break;
   }
   return;  
 } 
 WiFi_what_to_read=1;//always waiting!!!!
 WiFi_symbol_counter=0;
 #endif
}


//WiFi---------------------------------------------------------------------------------------------------------
void WiFi_send_command(uint8_t id, uint8_t Connection, uint16_t Send_N)
{
 uint8_t str[11];
 switch(id)
 {      
  case 0: //reset
  putstr(1,"AT+RST");         
  break;
  case 1: //echo off
  putstr(1,"ATE0");         
  break;
  case 2: //set current softAP ip addr
  putstr(1,"AT+CIPAP_CUR=\"192.168.0.100\"");         
  break;           
  case 3: //wifi name and password(none for now)
  putstr(1,"AT+CWSAP_CUR=\"2560DEMO\",\"\",1,0,1,0");         
  break; 
  case 4: //mode 2 softAP
  putstr(1,"AT+CWMODE=2");         
  break;         
  case 5: //multi access(have to be for a server}
  putstr(1,"AT+CIPMUX=1");         
  break;      
  case 6: //server start
  putstr(1,"AT+CIPSERVER=1,80");         
  break;         
  case 7: //close an ip connection IPC_N 0-4
  putstr(1,"AT+CIPCLOSE=");
  putchar1(Connection+48);//ascii or IPC_N+48         
  break;
  case 8: //send to WiFi_Con_N , WiFi_Send_N symbols          
  putstr(1,"AT+CIPSEND=");
  putchar1(Connection+48);
  putchar1(',');
  IntToStr(str,Send_N);
  putstr(1,str);
  break;
  case 9: //baudrate CURRENT not saved in eeprom       
  putstr(1,"AT+UART_CUR=9600,8,1,0,0");
  break;
  case 10:           
  putstr(1,"AT+UART_CUR=115200,8,1,0,0");
  break;      
  default: return;
 }
 putchar1(13);
 putchar1(10);
}


void WiFi_setup(void)
{
 UART_setup(1,1);
 T3_delay(1);//0.02s 
// WiFi_send_command(9,0,0);//9600
// T3_delay(4);
// UART_setup(1,0);
// T3_delay(4);//0.1s
 WiFi_send_command(1,0,0);//echo off
 T3_delay(4);//0.1s 
 WiFi_send_command(2,0,0);
 T3_delay(4);//0.1s  
 WiFi_send_command(3,0,0);
 T3_delay(4);//0.1s  
 WiFi_send_command(4,0,0);
 T3_delay(4);//0.1s
 WiFi_send_command(5,0,0);
 T3_delay(4);//0.1s 
 WiFi_send_command(6,0,0);
}

/*
�������� ������ � ������� esp-12

������� ����� �������� ���� ���

all command should ends with (13)(10)!!!!!!!
AT+UART_DEF=9600,8,1,0,0                    //��������
                                                                                       //answer
AT_RST                                                                                 //ready
ATE0                                                                                   //OK
AT+CWSAP_CUR="LIGHTHOUSE L200N1","",1,0,1(or 4),0 //��� ����������� ���� ��� ������                                                                   
AT+CIPAP_CUR="192.168.0.100"    //IP to enter in browser                               //OK
AT+CWMODE=2 //mode 2 softAP                                                            //OK
AT+CIPMUX=1 //multi                                                                    //OK
AT+CIPSERVER=1,80 //server start                                                       //OK, ERROR, no change

when client connects to wifi enter in browser 192.168.0.100 and there is connection to module

0,CONNECT


1,CONNECT
2,CONNECT
1,CLOSED
etc.... maybe have to set matrix CONNECTED[4]={1,0,0,0} // connected/not connected

+IPD,0,653:GET / HTTP/1.1    //CONNECTION 0 wants HTTP   
Host: 192.168.0.100
+a lot of other useless crap..........
the end of crap is (13)(10)(13)(10)

//controller should sent html file here using commands:

AT+CIPSEND=0,66     //send to 0, 66 symbols                //"> "(13)(10)
send 66 bytes                                              //"Recv 66 bytes"

//then close connection
AT+CIPCLOSE=0   //0-N of connection

//then comp/mobile send another GET like 

+IPD,0,411:GET /favicon.ico HTTP/1.1  //0 wants an icon 
//Just close it(AT+CIPCLOSE=0) cause you don't have any
//it can be a couple of times...

//then if user press a button or submit a form or anythuing
+IPD,1,687:GET /ON HTTP/1.1//
//here you have to send another piece of html.... and so on...

//when you submit:
+IPD,1,694:GET /?B=5 HTTP/1.1     //form name B and M from simple.html


AT+CIPSERVER=0 //stop server     //maybe to change the name or something else


//Parameters change requests examples
+IPD,0,694:GET /?N=LH200A HTTP/1.1
+IPD,0,691:GET /?B=100 HTTP/1.1
+IPD,0,691:GET /?M=8 HTTP/1.1
+IPD,1,686:GET /?I= HTTP/1.1
+IPD,1,702:GET /?E=%2B79514338436 HTTP/1.1
+IPD,1,702:GET /?R=%2B79513053591 HTTP/1.1



                    
//����� ��������� ������������� (5���)
<meta http-equiv="refresh" content="5">
*/