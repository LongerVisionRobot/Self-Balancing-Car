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


/*
 * Note:This file is stripped from main.c to make it more readable,there might
 * be some definition problem and won't pass the compliler.
 * */

#include"Time_Function.h"
#include <hidef.h>             /* common defines and macros */
#include "derivative.h"        /* derivative-specific definitions */
//#include "includes.h"
#include "ADC.h"

/*
 * A driver to read TSL1401 Linear CCD.
 * */
void RD_CCD(uchar channel)//adc0=L,adc1=R
{
  byte i=0,tslp=0;
  if(channel==0){

    DisableInterrupts;
    TSL_CLK=1;//��ʼ��ƽ��
    TSL_SI=0; //��ʼ��ƽ��
    Dly_us(1); //�������ʱ

    TSL_SI=1; //������
    TSL_CLK=0;//�½���
    Dly_us(1); //������ʱ

    TSL_CLK=1;//������
    TSL_SI=0; //�½���
    Dly_us(1); //������ʱ
    for(i=0;i<128;i++)
    {
      TSL_CLK=0;//�½���
      Dly_25ns(5); //������ʱ
      ADV[channel][tslp]=(byte)(ADCValue(channel));  //AD�ɼ�
      ++tslp;
      TSL_CLK=1;//������
      Dly_25ns(5); //������ʱ
    }
  } else if(channel==1){

    DisableInterrupts;
    TSL_CLK1=1;//��ʼ��ƽ��
    TSL_SI1=0; //��ʼ��ƽ��
    Dly_us(1); //�������ʱ

    TSL_SI1=1; //������
    TSL_CLK1=0;//�½���
    Dly_us(1); //������ʱ

    TSL_CLK1=1;//������
    TSL_SI1=0; //�½���
    Dly_us(1); //������ʱ
    for(i=0;i<128;i++)
    {
      TSL_CLK1=0;//�½���
      Dly_25ns(5); //������ʱ
      ADV[channel][i]=(byte)(ADCValue(channel));  //AD�ɼ�
      TSL_CLK1=1;//������
      Dly_25ns(5); //������ʱ
    }
  }
  EnableInterrupts;
}
