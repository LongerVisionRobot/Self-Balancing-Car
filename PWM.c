#include <hidef.h>            /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "stdio.h"


void PWM_Init(void)     //ԭ����ͨ��PWM#################################################
{   
  //*****�ҵ�� 0��1����********
  PWMCTL_CON01 = 1;
  PWME_PWME1 = 0;
  PWMPRCLK = 0X00;    //clockA 1��Ƶ:80Mhz
  //PWMSCLA = 5;        //��clock SA ����2*PWMSCLA = 10��Ƶ��pwm clock = clockA/10; 
  PWMPOL_PPOL1 = 1;   //������Ч�ڼ�Ϊ�͵�ƽ
  PWMCLK_PCLK1 = 0;   //ʱ��ΪSA
  PWMCAE_CAE1 = 0;    //�����
  PWMPER01 = 10000;   //PWMƵ�� = A/5000 = 16KHz
  PWMDTY01 = 0;   
  PWME_PWME1 = 1; 
  //******************************
 
  //*****�ҵ�� 4��5����********
  PWMCTL_CON45 = 1;
  PWME_PWME5 = 0;  
  PWMPOL_PPOL5 = 1;   //������Ч�ڼ�Ϊ�͵�ƽ
  PWMCLK_PCLK5 = 0;   //ʱ��ΪA
  PWMCAE_CAE5 = 0;    //�����
  PWMPER45 = 10000;   //PWMƵ�� = A/5000 = 16KHz
  PWMDTY45 = 0;   
  PWME_PWME5 = 1;
  
  //******************************
      //*****���� 2��3����********
  PWMCTL_CON23 = 1;
  PWME_PWME3 = 0;
  PWMPOL_PPOL3 = 1;   //������Ч�ڼ�Ϊ�͵�ƽ
  PWMCLK_PCLK3 = 0;   //ʱ��ΪSB
  PWMCAE_CAE3 = 0;    //�����
  PWMPER23 = 10000;   //PWMƵ�� = A/5000 = 16KHz
  PWMDTY23 = 0;   
  PWME_PWME3 = 1; 
  //******************************

   //*****���� 6��7����********
  PWMCTL_CON67 = 1;
  PWME_PWME7 = 0;  
  PWMPOL_PPOL7 = 1;   //������Ч�ڼ�Ϊ�͵�ƽ
  PWMCLK_PCLK7 = 0;   //ʱ��ΪB
  PWMCAE_CAE7 = 0;    //�����
  PWMPER67 = 10000;   //PWMƵ�� = A/5000 = 16KHz
  PWMDTY67 = 0;   
  PWME_PWME7 = 1; 
  //******************************
}