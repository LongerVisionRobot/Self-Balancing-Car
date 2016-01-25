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

//---------------------------------------------------------------------
//�� �� ��:ADCInit                                                      
//��    ��:A/Dת����ʼ��,����A/Dת��ʱ��Ƶ��Ϊ1MHz                      
//��    ��:��                                                           
//��    ��:��                                                           
//---------------------------------------------------------------------
void ADC_Init(void)
{

  //ATD0CTL4: SMP2=1,SMP1=1,SMP0=1,PRS4=0,PRS3=1,PRS2=1,PRS1=1,PRS0=1 
  //ATD0CTL4 = 0b11101111; //���ò���ʱ���Ƶ��,fATDCLK=fBUS/(2 ��(PRS + 1))
  ATD0CTL4 = 0b11100100; //���ò���ʱ���Ƶ��,fATDCLK=fBUS/(2 ��(PRS + 1)) 
  
  //ATD0CTL3: DJM=1,S8C=0,S4C=0,S2C=0,S1C=1,FIFO=0,FRZ1=0,FRZ0=0 
  ATD0CTL3 = 0b10001000; //��������Ҷ��룬ÿ�����е�ת������Ϊ1
   
  //ATD0CTL0: ??=0,??=0,??=0,??=0,WRAP3=1,WRAP2=1,WRAP1=1,WRAP0=1 
  ATD0CTL0 = 0b00000001; //ADѭ���ɼ���AN1�󼴿�  
  
  //ATD0CTL1: ETRIGSEL=0,SRES1=0,SRES0=0,SMP_DIS=0,ETRIGCH3=1,ETRIGCH2=1
  //,ETRIGCH1=1,ETRIGCH0=1 
  ATD0CTL1 = 0b00001111; //8λ�ֱ��ʣ�����ǰ��ж���ڲ���������   
  
  //ATD0CTL2: ??=0,AFFC=1,ICLKSTP=0,ETRIGLE=0,ETRIGP=0,ETRIGE=0,ASCIE=0,
  //ACMPIE=0 
  ATD0CTL2 = 0b01000000; //�½��ش������������ⲿ�źţ�����ATD�Ƚ��ж�����      

}

void setADC8bit() {
  
  ATD0CTL1 = 0b00001111; //8λ�ֱ��ʣ�����ǰ��ж���ڲ���������
  
}

void setADC12bit() {
  
  ATD0CTL1 = 0X4F; //12λ�ֱ���,�����ⲿ����,����ǰ��ж���ڲ���������
  
}

void ADC_Init0(void)
{
  ATD0CTL0 = 0X00; //ADѭ���ɼ���AN0 (ATD0CTL5_MULT=1)  
  ATD0CTL1 = 0X4F; //12λ�ֱ���,�����ⲿ����,����ǰ��ж���ڲ���������
  ATD0CTL2 = 0b01000000; //��������,�ⲿ�ź��½��ش�������ֹ�ⲿ��������ֹAD�ж�, ����ATD�Ƚ��ж�
  ATD0CTL3 = 0b10001000; //��������Ҷ��룬ת��1��ͨ��
  ATD0CTL4 = 0b00100001; //���ò���ʱ���Ƶ��,fATDCLK=fBUS/(2 ��(PRS + 1))
}
int ADChannelx(unsigned char Channel)
{  
  ATD0CTL5_Cx = Channel;
  while(!ATD0STAT0_SCF);
  return ATD0DR0;
}

//---------------------------------------------------------------------
//�� �� ��:ADCValue                                                     
//��    ��:1·A/Dת������,��ȡͨ��channel��A/Dת�����                  
//��    ��:channel = ͨ����                                             
//��    ��:��ͨ����12λA/Dת�����                                      
//---------------------------------------------------------------------
uint ADCValue(uchar channel)
{
  //�ݴ�A/Dת���Ľ��
  uint temp;                        
	ATD0CTL5 = channel;
	//ȡA/Dת�����                                  
  while(!ATD0STAT0_SCF);
  temp = ATD0DR0;
	return  temp;
}