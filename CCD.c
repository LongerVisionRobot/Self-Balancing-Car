#include"Time_Function.h"
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
//#include "includes.h"
#include "ADC.h"
//#include "CCD.h"
/*void CalculateFZ (void) 
{
  for(CCDt=0;CCDt<128;CCDt++) 
	      {
	        CCDa=CCDa+ADV[CCDt];
	      }
	CCDa=CCDa/128;
	FZ=CCDa*0.35;
}

void GetBlack (void) 
{
    for(CCDt=64;CCDt<120;CCDt++)  //Rblack
	      {
	        if(ADV[CCDt]-ADV[CCDt+DCCD]>FZ) 
	        {
	          Rblack=CCDt+DCCD;
	          for(CCDt=Rblack;CCDt<128;CCDt++) 
	          {
	            ADV[CCDt]=4;
	          }
	        }
	      }
	  for(CCDt=64;CCDt>=0;CCDt--)  //Lblack
	      {
	        if(ADV[CCDt]-ADV[CCDt-DCCD]>FZ) 
	        {
	          Lblack=CCDt-DCCD;
	          for(CCDt=Lblack;CCDt>=0;CCDt--) 
	          {
	            ADV[CCDt]=4;
	          }
	        }
	      }
}


void RD_TSL(void) 
{
  byte i=0,tslp=0;
  
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
    Dly_us(10); //������ʱ
    ADV[tslp]=(byte)(ADCValue(0)>>4);  //AD�ɼ�
    ++tslp;
    TSL_CLK=1;//������ 
    Dly_us(10); //������ʱ    
  }   
}
*/