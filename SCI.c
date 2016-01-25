/*
Copyright (c) <2013> <Malcolm Ma>

Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction, including without limitation the rights 
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
copies of the Software, and to permit persons to whom the Software is 
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
SOFTWARE.
*/

#include <hidef.h>            /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "stdio.h"
#include "math.h"
#include "SCI.h"
//---------------------------------------------------------------------
// �������ܣ�UART0_Init��ʼ��
// ��ʽ������  ��
// ��������ֵ����   
//---------------------------------------------------------------------
int rx_buffer_head = 0;
int rx_buffer_tail = 0;
char rx_buffer[RX_BUFFER_SIZE]={0};


void UART0_Init(void)
{
  SCI0CR1 = 0x00; 
  SCI0CR2 = 0x2C;     //�����ж�ʹ�ܣ����ͽ���ʹ��
  SCI0BD  = 130;       //521--9600
  //SCI0BD  = 130;     //���������ó�38400,fbus=80M
                      //When IREN = 0 then 
                      //SCI baud rate = SCI bus clock / (16 x SBR[12:0])
}



//---------------------------------------------------------------------
// �������ܣ�SCI0����һ���ֽ�����
// ��ʽ������  byte ch�����͵�һ���ֽ�����
// ��������ֵ����   
//---------------------------------------------------------------------


void uart0_putchar(unsigned char ch)
{
 	if (ch == '\n')  
  {
      while(!(SCI0SR1&0x80)) ;     
      SCI0DRL= 0x0d;       				 //output'CR'
	    return;
  }
  while(!(SCI0SR1&0x80)) ; 		    //keep waiting when not empty  
  SCI0DRL=ch;	
}

//---------------------------------------------------------------------
// �������ܣ�SCI0�����ַ�������
// ��ʽ������   byte *pBuff     ���ͻ�����
//              int Length �����ֽڵĳ��� 
// ��������ֵ����   
//---------------------------------------------------------------------
void uart0_sendpacket(byte *pBuf,int pBuf_Length) 
{
  int i;
  for(i=0;i<pBuf_Length;i++)
  {
    while(!(SCI0SR1&0x80));
    SCI0DRL=*(pBuf+i); 
  }
}
 

void uart0_putstr(char ch[])
{
  unsigned char ptr=0;
  while(ch[ptr]){
      uart0_putchar((unsigned char)ch[ptr++]);
  }     
  
}
void uart0_putf(float num)
{
  char buf[20]={0};
  (void)far_sprintf(buf,"%.3f,",num);
  uart0_putstr(buf);
}



char uart0_read()
{
  char data  ;
  if (rx_buffer_head == rx_buffer_tail) {
    return SERIAL_NO_DATA;
  } else {
    data = rx_buffer[rx_buffer_tail];
    rx_buffer_tail++;
    if (rx_buffer_tail == RX_BUFFER_SIZE) { rx_buffer_tail = 0; }
    return data;
  }
}
char uart0_available() {
    if (rx_buffer_head == rx_buffer_tail) {
    return SERIAL_NO_DATA;
    return 1;
  }
}
unsigned char SciRead(){
    
    if(SCI0SR1_RDRF==1)     //�������ݴ�λ�ƼĴ������䵽SCI���ݼĴ���
      {      SCI0SR1_RDRF=1;     //��ȡ���ݼĴ����ὫRDRF���  ������λ 
           return SCI0DRL;        //�������ݼĴ���������
        }
        
}


void SciWrite2(unsigned char sendchar){
    while (!(SCI0SR1&0x80));    
    SCI0DRH=0;    
    SCI0DRL=sendchar;
    }


#pragma CODE_SEG NON_BANKED

interrupt 20 void SCI_RX_IRS(void){
byte RxData,RX;
byte next_head;
DisableInterrupts;
RX=SCI0SR1;
//��״̬�Ĵ�����Ϊ������׼��
RxData=(byte)SCI0DRL; //�����ռĴ�����ֵ
//uart0_putchar(RxData);
next_head = rx_buffer_head + 1;
if (next_head == RX_BUFFER_SIZE) { next_head = 0; }
if (next_head != rx_buffer_tail) {
        rx_buffer[rx_buffer_head] = RxData;
        rx_buffer_head = next_head;  
}
EnableInterrupts;
}  




