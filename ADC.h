#ifndef   ADC_H
#define   ADC_H

/*
*********************************************************************************************************
* Description: No
*********************************************************************************************************
*/
void ADC_Init(void);
uint ADCValue(uchar channel);//adc��ȡ
void setADC8bit();
void setADC12bit();


#endif