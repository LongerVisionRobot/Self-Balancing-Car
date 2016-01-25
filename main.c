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
-------------------------------------------
Code Warrior 5.0/1
Target : MC9S12XS128
Crystal: 16.000Mhz
busclock:80.000MHz
pllclock:160.000MHz  
  
�����˲�
˫�����ǲ���ȡƽ��
��ʱ����ʱ
����Ȩֵ
 �����ٶȼ���Сʱ
  0.99<weight<0.998
 ����̫��ʱ
  weight=1
  
����ת��һȦ�������� R=1139 1119 1120.5  1121  L= 1118.5 1105 1119   avr=1120
����ֱ��50.5mm  �ܳ�=158.65mm
MMperPulse=158.65/1120=0.14165
PulseperMM=7.05965
  
�ٶȱջ�������1.5m/sײǽ����
ȥ�����ٶ�ǰ�����жϣ�ֻ��ǰ�����ܺ���
ԭ���ٷ��ķ����жϷ��������⣬�ᵼ���ٶȲ��ȶ�
�ٶȿ��Ʒֶ�P

ǰ��ʽCCD����
Сǰհ45����ǰհ60
Сǰհ���׳���
31M/22��~28��

С���ܶ�ʱ·��������

����������ʱ�䣺
CCD 8bit ����         572us
AccGyro  ����+����    784us
�����ٶȿ��ơ����  208us

�ٶȲ���
ǰհ40cm@68��
38m--31.5s
�ȶ��Բ���

38m--26s

��·������


������PID������ֱ��Ч�����ã����Ǽ������ѣ���
CCD������ֵ�˲�
���������߼�⣬6��������

P=36 SetSpeedMM=1800



********************************************************/

#include "derivative.h"
#include "stdio.h"
#include "math.h"
#include "SCI.h"
#include "ADC.h"
#include "Time_Function.h"
#include "PWM.h"
#include "includes.h"

                                                      





//LOW_PASS,Fs=200,HAMMING,wp=3,ws=9
#define FILTERTAPS 5
float coef[FILTERTAPS] = {0.001193,0.010720,0.026164,0.026164,0.010720};
float gain=13.340246;    
float values[FILTERTAPS] = {0}; 

float FIR(float in){
   static byte k;
   byte i = 0; 
   float out = 0;
   values[k] = in; 
   for (i=0; i<FILTERTAPS; i++) {            
     out += coef[i] * values[(i + k) % FILTERTAPS];                          
    }
    out *= gain;                        
    k = (k+1) % FILTERTAPS;                  
    return out;                        
}





//LOW_PASS,Fs=40,RECTANGLE,wp=10,ws=11
#define FILTERTAPS2 5
float coef2[FILTERTAPS2] = {0.000000,0.318310,0.500000,0.500000,0.318310};
float gain2=0.611015;


float values2[FILTERTAPS2] = {0}; 

float FIR2(float in){
   static byte k;
   byte i = 0; 
   float out = 0;
   values2[k] = in; 
   for (i=0; i<FILTERTAPS2; i++) {            
     out += coef2[i] * values2[(i + k) % FILTERTAPS2];                          
    }
    out *= gain2;                        
    k = (k+1) % FILTERTAPS2;                  
    return out;                        
}






//LOW_PASS,Fs=20,HAMMING,wp=3,ws=9
#define FILTERTAPS3 5
float coef3[FILTERTAPS3] = {0.006055,0.092153,0.261644,0.261644,0.092153};
float gain3=1.401247;
float values3[FILTERTAPS3] = {0}; 

float FIR3(float in){
   static byte k;
   byte i = 0; 
   float out = 0;
   values3[k] = in; 
   for (i=0; i<FILTERTAPS3; i++) {            
     out += coef3[i] * values3[(i + k) % FILTERTAPS3];                          
    }
    out *= gain3;                        
    k = (k+1) % FILTERTAPS3;                  
    return out;                        
}





//LOW_PASS,Fs=20,HAMMING,wp=3,ws=9
#define FILTERTAPS4 5
float coef4[FILTERTAPS4] = {0.006055,0.092153,0.261644,0.261644,0.092153};
float gain4=1.401247;
float values4[FILTERTAPS4] = {0}; 

float FIR4(float in){
   static byte k;
   byte i = 0; 
   float out = 0;
   values4[k] = in; 
   for (i=0; i<FILTERTAPS4; i++) {            
     out += coef4[i] * values4[(i + k) % FILTERTAPS4];                          
    }
    out *= gain4;                        
    k = (k+1) % FILTERTAPS4;                  
    return out;                        
}

//LOW_PASS,Fs=20,HAMMING,wp=3,ws=9
#define FILTERTAPS5 5
float coef5[FILTERTAPS5] = {0.006055,0.092153,0.261644,0.261644,0.092153};
float gain5=1.401247;
float values5[FILTERTAPS4] = {0}; 

float FIR5(float in){
   static byte k;
   byte i = 0; 
   float out = 0;
   values5[k] = in; 
   for (i=0; i<FILTERTAPS5; i++) {            
     out += coef5[i] * values5[(i + k) % FILTERTAPS5];                          
    }
    out *= gain5;                        
    k = (k+1) % FILTERTAPS5;                  
    return out;                        
}


//LOW_PASS,Fs=200,HAMMING,wp=3,ws=9
#define FILTERTAPS6 5
float coef6[FILTERTAPS6] = {0.001193,0.010720,0.026164,0.026164,0.010720};
float gain6=13.340246;    
float values6[FILTERTAPS6] = {0}; 

float FIR6(float in){
   static byte k;
   byte i = 0; 
   float out = 0;
   values6[k] = in; 
   for (i=0; i<FILTERTAPS6; i++) {            
     out += coef6[i] * values6[(i + k) % FILTERTAPS6];                          
    }
    out *= gain;                        
    k = (k+1) % FILTERTAPS6;                  
    return out;                        
}


long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void CalculateCCD0 (void) 
{
   int i; 
   int tempEdge;
   if(CCDDebugSwitch2==1) {
   for(i=1;i<127;i++){
    
    CCDBuf[i]=mid(ADV[0][i-1],ADV[0][i],ADV[0][i+1]);  
    CCDBuf2[i]=CCDBuf[i]; 
    } 
   }
   else    
   for(i=0;i<128;i++){
    
    CCDBuf[i]=ADV[0][i];  
    CCDBuf2[i]=ADV[0][i]; 
   }
   
      for(CCDt=0,CCDAvr0=0;CCDt<128;CCDt++) 
          {
            CCDAvr0=CCDa+CCDBuf2[CCDt];
          }  
      CCDAvr0=CCDAvr0/128;
      FZ=CCDa*FZBL;
     
     if(angleFilter2>OriginPoint-10&&angleFilter2<OriginPoint+15) //-74~-49
        if(CCDAvr0<Threshold*2/3)
           obstacleSign=true;
       
       
       for(CCDt=0,tempEdge=128,CCDEdge=0;CCDt+DCCD<128;CCDt++) {
         if(abs(CCDBuf2[CCDt]-CCDBuf2[CCDt+DCCD])>FZ) 
          {
          if(abs(tempEdge-CCDt)>DCCD*2) {
            tempEdge=CCDt;
            edge[CCDEdge]=CCDt;
            CCDEdge++;
          }
          }
       }
      if(CCDEdge>5&&(millis()-scratchLineTimer)>10000) {
        scratchLineCount++;
        scratchLine=true;
      }
      
      /*
      if(CCDBuf2[LineCenter]>CCDa*0.71)
        if(CCDBuf2[LineCenter+trackWidth/4]<CCDa*0.71)
           if(CCDBuf2[LineCenter-trackWidth/4]<CCDa*0.71)
              scratchLine=true;
      */ 
        for(CCDt=LineCenter;CCDt+DCCD<128;CCDt++)  //Rblack
        {
          if(CCDBuf2[CCDt]-CCDBuf2[CCDt+DCCD]>FZ) 
          {
            Rblack=CCDt+DCCD;
             for(CCDt=Rblack;CCDt<128;CCDt++) 
            {
              CCDBuf2[CCDt]=2;
            }  
          }
        }
    for(CCDt=LineCenter;CCDt-DCCD>=0;CCDt--)  //Lblack
        {
          if(CCDBuf2[CCDt]-CCDBuf2[CCDt-DCCD]>FZ) 
          {
            Lblack=CCDt-DCCD;
            for(CCDt=Lblack;CCDt>=0;CCDt--) 
            {
              CCDBuf2[CCDt]=2;
            }  
          }
        }
        
        if(Lblack>5&&Rblack<123)
         trackWidth=Rblack-Lblack;
        
        if(Lblack>50)
         LineCenter=Lblack+trackWidth/2;
        else if(Rblack<78)
         LineCenter=Rblack-trackWidth/2;
        else
         LineCenter=(Lblack+Rblack)/2; 
        
        
        if(LineCenter+DCCD>125)
          LineCenter=125;
        else if(LineCenter<5)
          LineCenter=5;
        LastC1=LineCenter;

        LastC3=LastC2;
        LastC2=LastC1;

        
        LineCenter=FIR4(LineCenter);

}


 void SendCCDData(byte arr[]) 
{
  int i;
 for(i=(64-WINDOW_WIDTH/2); i<(64+WINDOW_WIDTH/2); i++)
    {
#if(PRINT_AD==1)          //���ڷ���ADֵ������������CCD��������
      if(arr[i]==0xFF) 
        arr[i] = 0xFE; //����FF��FE�滻����
      uart0_putchar(arr[i]);
#else                     //���ڷ��Ͷ�ֵ���������ô��ڵ���
      if(arr[i]>THRESHOLD)
        uart0_putchar(1);
      else
        uart0_putchar(0);
#endif     
    }
    
    uart0_putchar(0xFF); 
}
  

void Port_Init(void)  //#############################################
{ 
  //DDRA=0XFF;  
  //DDRB = 0XFF;      //ԭ��
  //PORTB = 0X00;
  
  
  DDRA=0XFF;  
  DDRB = 0X00;      //����ʵ��
  DDRH=0x00;
}



//IOC7/PT7���ڼ����ұ�����������������
void BMQ_Init(void)
{   
  TCNT = 0x00;
  PACTL= 0xC0;//����PAC
  TIE  = 0x00;//ÿһλ��Ӧ��Ӧͨ����ֹ�ж�
  PACNT = 0;
  BMQR=0;
} 


void getspeed (void) 
{
DisableInterrupts;
Lspeed=PORTB;
Rspeed=PACNT;
PACNT=0;
BMQR=1;
Dly_us(1);
BMQR=0;
//if(speed<0) 
  {
    Lspeed=-Lspeed;
    Rspeed=-Rspeed;
  }

  LspeedJF=LspeedJF+Lspeed;
  RspeedJF=RspeedJF+Rspeed;
  EnableInterrupts;
}

float timer1,time1,timer2,time2,timer3,time3;
float testGyroZ1=0,testGyroX1=0;
void getAD (void) 
{
    unsigned int tlycs,jiao;
    float temp=0;
    setADC12bit(); 
    time2=micros();
    //DisableInterrupts;
    tlycs=ADChannelx(5)+ADChannelx(5)+ADChannelx(5)+ADChannelx(5)+ADChannelx(5) 
         +ADChannelx(5)+ADChannelx(5)+ADChannelx(5)+ADChannelx(5)+ADChannelx(5);
    temp=tlycs/10.0;
    testGyroZ1=temp;
    gyro=(temp-TLYLDZ1)*TLYBL; 
     
    
    tlycs=ADChannelx(6)+ADChannelx(6)+ADChannelx(6)+ADChannelx(6)+ADChannelx(6) 
         +ADChannelx(6)+ADChannelx(6)+ADChannelx(6)+ADChannelx(6)+ADChannelx(6);
    temp=tlycs/10.0;
    testGyroX1=temp;
    
     
     
     
     
     jiao=ADChannelx(4)+ADChannelx(4)+ADChannelx(4) + ADChannelx(4)+ADChannelx(4)
          +ADChannelx(4)+ADChannelx(4)+ADChannelx(4) + ADChannelx(4)+ADChannelx(4);           
    Aangle=jiao/20.0;                
    Aangle=(Aangle-JSDLD)*0.13;//0.12;  
    angleAZ=jiao/10.0-accOffset;
    
    jiao=ADChannelx(2)+ADChannelx(2)+ADChannelx(2) + ADChannelx(2)+ADChannelx(2)
          +ADChannelx(2)+ADChannelx(2)+ADChannelx(2) + ADChannelx(2)+ADChannelx(2); 
    angleAX=jiao/10.0-accOffset;  
    
    jiao=ADChannelx(3)+ADChannelx(3)+ADChannelx(3) + ADChannelx(3)+ADChannelx(3)
          +ADChannelx(3)+ADChannelx(3)+ADChannelx(3) + ADChannelx(3)+ADChannelx(3); 
    angleAY=jiao/10.0-accOffset;
    
    timer2=micros()-time2;
    //EnableInterrupts;
}

  
  


float TurnSpeed=0;
float gyroTimeTest=0; 
float tempX,tempY,tempZ;

void AngleCalculate (void) 
{
    unsigned long time=0;   
    
    time1=micros();
    
    tempX=angleAX*5/4096*1000/AccSense;
    tempY=angleAY*5/4096*1000/AccSense;
    tempZ=angleAZ*5/4096*1000/AccSense;
    
    
    angleA=-atan2(angleAX,angleAZ)*180/PI;
    gravity=sqrt(tempX*tempX+tempY*tempY+tempZ*tempZ);
     //angleA2=angleAZ/AccSense;
    angleA=FIR(angleA); 
    if(debug) {
    gravityError=fabs(gravity-gravityG);
    /*
    if(gravityError>gravityVibrationGate)// avoid vibration
    {
     weightFlag=5; 
     weight=1;
    } else if(gravityError<gravityGate)  //trust acc more
    {
     weightFlag=0;
     weight=weight2;
    }
    else {
     weightFlag=1; 
     weight=weight1;
    }
    */
    if(gravityError>gravityVibrationGate)// avoid vibration
    {
     weightFlag=10; 
     weight=1;
    } else{
      weight=weight2+(weight1-weight2)*gravityError/gravityVibrationGate;
      weightFlag=(weight-weight2)*100;
      }
    }
    
    
    time=micros()-gyroTimer;
    gyroTimeTest=time;
    gyroTimer=micros();
    if(time>50000)  //50ms,timeout
     angleG2=0;     //large delay will effect the gyro integration
    else
     angleG2=(testGyroZ1-TLYLDZ1)*5/4096*1.5/5.1/GyroSense2*time/1000;
    
    //TurnSpeed=(testGyroX1-TLYLDX1+testGyroX2-TLYLDX2)*GyroSense/2.0;
    
    
    //angleG=(testGyro-TLYLD)*0.00911;
    //angleFilter=angleA*(1-weight)+(angleG+angleFilter)*weight;
    angleFilter2=angleA*(1-weight)+(angleG2+angleFilter2)*weight;
    //angleFilter=angleG+angleFilter;
    
    timer1=micros()-time1;
}



void shutdown() {

    MotorEnable=false;
    PWMDTY01=0;
    PWMDTY23=0;
    PWMDTY67=0; 
    PWMDTY45=0;
}
void turnon(){
   MotorEnable=true;
}
void speedout (void) 
{
 
  if(!MotorEnable)
  return;

 if(fabs(Setpoint-angleFilter2)>45) {
  LOUT=0;
  ROUT=0;
 }
 
 if(LOUT==0)
  {
    PWMDTY01=0;
    PWMDTY23=0;  
  } 
  else if(LOUT>0) 
    { 
    PWMDTY01=0;
    PWMDTY23=LOUT+M_SQL;  //L
    } 
  else if(LOUT<0) 
    { 
    PWMDTY23=0;
    PWMDTY01=M_SQL-LOUT;
    }
  if(ROUT==0) 
  {
    PWMDTY67=0; 
    PWMDTY45=0;
  } 
  else if(ROUT>0) 
  {
    PWMDTY45=ROUT+M_SQR;  
    PWMDTY67=0;
  }
  
  else if(ROUT<0) 
  {
    PWMDTY45=0; 
    PWMDTY67=M_SQR-ROUT;
  }
}


   

void Interrupt_Priority_Set(void){
    INT_CFADDR=0x70;
    INT_CFDATA5=0x05;
    INT_CFADDR=0xD0;
    INT_CFDATA3=0x07;
}

float FIROutput;

void printout (void) 
{ 
  
  /*
  uart0_putf(angleA);
  uart0_putf(angleFilter2);
  uart0_putf(Output);
  uart0_putf(FIROutput);
  uart0_putf(testGyroZ1);
  uart0_putf(testGyroZ2);
   */
  uart0_putf(angleA);
  uart0_putf(angleFilter2);
  //uart0_putf(g_fSpeedControlOutNew);
  uart0_putf(64);
  uart0_putf(LineCenter);
  uart0_putf(LineCenter1);
  
  uart0_putf(fspeed);
  uart0_putf(g_fCarSpeed*MMperPulse*1000/(5*scPeriod));
  
  
  uart0_putstr(" \n\r");  
}


 void PID() {
 int k=1;
 Error=Setpoint-Input;
 Error=FIR3(Error);
 k=map(Error,-180,180,0,60);
 ValueK=k;
// dErr=angleG2;      //kp=1600,kd=4000;
 dErr=testGyroZ1-TLYLDZ1;   //kp=1900,kd=20;
  {
    /*
    if(fabs(Setpoint-Input)<1) 
      Output=kp*Error*0.3-kd*dErr*0.2;
    else if(fabs(Setpoint-Input)<3) 
      Output=kp*Error*0.8-kd*dErr*0.5;
    else if(fabs(Setpoint-Input)>5)
      Output=kp*Error*1.5-kd*dErr*1.2;
    else   */ 
    //Output=kp*Error*tangent[k]-kd*dErr*tangent[k];
    Output=kp*Error*k-kd*dErr*k;
    //Output=kp*Error*30-kd*dErr*30;
    val_kp=kp*Error;
    val_kd=kd*dErr;
  }
if(fabs(Error)>45)
 Output=0;

LastErr=Error;
}

void SpeedControl (void) 
{
  float fP, fDelta;
  float fI;
  float fD;
  float P;
  
  static lastErr=0;

  
  g_fCarSpeed = (LspeedJF+RspeedJF) / 2;
  LspeedJF=RspeedJF=0;
  
  g_fCarSpeed*=1.0;
  g_fCarSpeed=FIR2(g_fCarSpeed);

  /*
  if(fabs(g_fCarSpeed*MMperPulse*scPeriod/1000-SetSpeedMM)<100)//300
   P=SPEED_CONTROL_P*0.2;
  else if(fabs(g_fCarSpeed*MMperPulse*scPeriod/1000-SetSpeedMM)<150) //500
   P=SPEED_CONTROL_P*0.3;
  else if(fabs(g_fCarSpeed*MMperPulse*scPeriod/1000-SetSpeedMM)<200) //500
   P=SPEED_CONTROL_P*0.4;
  else if(fabs(g_fCarSpeed*MMperPulse*scPeriod/1000-SetSpeedMM)<250) //500
   P=SPEED_CONTROL_P*0.5;
  else if(fabs(g_fCarSpeed*MMperPulse*scPeriod/1000-SetSpeedMM)<300) //500
   P=SPEED_CONTROL_P*0.6;
  else if(fabs(g_fCarSpeed*MMperPulse*scPeriod/1000-SetSpeedMM)<350) //500
   P=SPEED_CONTROL_P*0.7;
  else if(fabs(g_fCarSpeed*MMperPulse*scPeriod/1000-SetSpeedMM)<400) //500
   P=SPEED_CONTROL_P*0.8;
  else 
   P=SPEED_CONTROL_P;
  */ 
  P=SPEED_CONTROL_P;
  fDelta = SetSpeed- g_fCarSpeed;
  fDelta=FIR5(fDelta);
  
  if(fDelta>=0)
    k=map(fDelta,0,1000,0,45);
  else
    k=map(fDelta,0,-1000,0,45);
  speedK=tangent[k];  
   
  fP = fDelta * P*speedK;
  fI = fDelta * SPEED_CONTROL_I*speedK;
  fD = (fDelta-lastErr) * SPEED_CONTROL_D*speedK;
  lastErr=fDelta;
  g_fSpeedControlIntegral += fI;
  
  

  g_fSpeedControlOutOld = g_fSpeedControlOutNew;

  g_fSpeedControlOutNew = fP + g_fSpeedControlIntegral-fD;
  
  if(g_fSpeedControlOutNew > SPEED_CONTROL_OUT_MAX) 
    g_fSpeedControlOutNew = SPEED_CONTROL_OUT_MAX;
  if(g_fSpeedControlOutNew < SPEED_CONTROL_OUT_MIN)   
    g_fSpeedControlOutNew = SPEED_CONTROL_OUT_MIN;
  
  
  }
  
 void setSpeed(float myspeed) {
 
 SetSpeed=myspeed*PulseperMM*scPeriod*5/1000; 
 SetSpeed=0;
}


 void SpeedControlOutput(void) 
{
  float fValue;
  fValue = g_fSpeedControlOutNew - g_fSpeedControlOutOld;
  g_fSpeedControlOut=fValue * (g_nSpeedControlPeriod + 1) /scPeriod + g_fSpeedControlOutOld;
}


void DirectionControl(void) 
{
  //float DError1;
  int kk;
  float DErrorMult=1;
  g_fDirectionControlOutOld = g_fDirectionControlOutNew;
  DError=LineCenter-(64-3);
  //DError1=LineCenter1-64;
  
  /*
  if(fabs(DError)<6)
     DErrorMult*=1.3;
  else if(fabs(DError)<12)
     DErrorMult*=1.5;
  else if(fabs(DError)<18)
     DErrorMult*=1.6;
  else if(fabs(DError)<24)
     DErrorMult*=1.7;
  else if(fabs(DError)<30)
     DErrorMult*=1.8;
  else if(fabs(DError)>30)
     DErrorMult*=1.8;
  
   */
  DDError=testGyroX1-TLYLDX1;
  //kk=map(fabs(DDError),0,65,0,45);
  //DErrorMult=tangent[kk];
  g_fDirectionControlOutNew=DError*KDIR*DErrorMult-DDError*DDIR*DErrorMult;
  DLastError=DError; 
  /*
  fspeed=SetSpeedMM+fabs(DError*6);
  if(fspeed>-1600)
   fspeed=-1600;
  //setSpeed(fspeed); 
  */
  
}


void DirectionControlOutput(void) {
  float fValue;
  fValue = g_fDirectionControlOutNew - g_fDirectionControlOutOld;
  g_fDirectionControlOut=fValue * (g_nDirectionControlPeriod + 1) /dcPeriod + g_fDirectionControlOutOld;
  //g_fDirectionControlOut=0;   
}

void SpeedOutCalculate (void) 
{
  int i;
  float temp=0;
  speed=Output-g_fSpeedControlOut;
  
  //g_fDirectionControlOut=0;//for ccd test only
  LOUT=-(speed+g_fDirectionControlOut);
  ROUT=-(speed-g_fDirectionControlOut);
}


int samplecounter=0,sampletime=10, sampleSign=1;
float  tempGyroOffsetZ1=0,tempGyroOffsetZ2=0,tempGyroOffsetX1=0,tempGyroOffsetX2=0;
float tempAccX=0,tempAccY=0,tempAccZ=0;

void AccGyroCalibration() {
   
  for(;samplecounter<sampletime;samplecounter++) {
    getAD();
    tempGyroOffsetZ1+=testGyroZ1;
    tempGyroOffsetX1+=testGyroX1;
    tempAccX+=angleAX;
    tempAccY+=angleAY;
    tempAccZ+=angleAZ;
    //Dly_ms(1);
  }
  gyroOffset=tempGyroOffsetZ1/sampletime;
  
  TLYLDZ1=tempGyroOffsetZ1/sampletime;
  TLYLDZ2=tempGyroOffsetZ2/sampletime;
  TLYLDX1=tempGyroOffsetX1/sampletime;
  TLYLDX2=tempGyroOffsetX2/sampletime;
  
  tempAccX/=sampletime;
  tempAccY/=sampletime;
  tempAccZ/=sampletime;
  tempAccX=tempAccX*5/4096*1000/AccSense;
  tempAccY=tempAccY*5/4096*1000/AccSense;
  tempAccZ=tempAccZ*5/4096*1000/AccSense;
  gravityG=sqrt(tempAccX*tempAccX+tempAccY*tempAccY+tempAccZ*tempAccZ);
  

  uart0_putstr("\ngravityG=");
  uart0_putf(gravityG);
  uart0_putchar('\n');
  getAD ();
  angleFilter2=-atan2(angleAX,angleAZ)*180/PI;
  uart0_putstr("\n Acc Gyro Init Done\n");
  
}

int mid(int a,int b,int c) {
  
 int i,j,t;
 int arr[3];
 arr[0]=a;
 arr[1]=b;
 arr[2]=c;
 for(i=0;i<3;i++)
  for(j=i+1;j<3;j++)
   if(arr[i]>arr[j]) {
    t=arr[i];
    arr[i]=arr[j];
    arr[j]=t;
   }
 return arr[1]; 
  
}

void CCDCalibration() {

unsigned long tt=micros();
int i;
RD_CCD(0);
for(i=0;i<10;i++) {
    Dly_ms(ccdMultiple*5-1);
    RD_CCD(0);
    CalculateCCD0();
    Threshold+=CCDAvr0;
}
Threshold/=10;
obstacleSign=false;
}


char sendSign=0,updateSign=0;
unsigned long speedTimer=0;
unsigned long tt1,ttr1;
unsigned char lastSwith,swithChange;
  void main()
{ 
  //char c,cc; 
  char temp;
  char read;
  int i;
  setbusclock_80M();
  Interrupt_Priority_Set();    
  Port_Init();     
  PWM_Init();        
  BMQ_Init(); 
  UART0_Init();
  ADC_Init0();
  setADC12bit();
  PIT_Init();
  EnableInterrupts;
  DisableControlPIT
  EnableTimerPIT;
  
  Dly_ms(2000);
  CCDCalibration();
  AccGyroCalibration();
  
  //gyroTimer=micros();//large delay will effect the gyro integration
  //while(1){speedout ();getspeed();Dly_ms(5);}  //����������������
  speedTimer=millis();
 
  
  EnableControlPIT;
  for(;;)
  {

    //LOUT=-speed;
    //ROUT=-speed;
    //speedout (); 
    
    time3=micros();
    /*DisableInterrupts;
    for(i=0;i<128;i++)
     CCDBuf[i]=ADV[i];
    EnableInterrupts;
    SendCCDData(CCDBuf);
    //printout ();
    */
    if(CCDDebugSwitch==0)
      printout ();
     
    if(swithChange==true){
      Dly_ms(2000);
      swithChange=false;
    }
    
    
    //if(sendSign){
      if(CCDDebugSwitch==1)
      SendCCDData(CCDBuf);
      sendSign=0;
    //} 
    if(scratchLine==true) {
      if(StopCarOn==true) {
        Dly_ms(400);
        shutdown();
        while(StopCarOn==true);
        Dly_ms(1000);
      }
      scratchLine=false;
    }
    timer3=micros()-time3;
    timer3=micros()-time3;
     //speedout (); 
  }
}


#pragma CODE_SEG __NEAR_SEG NON_BANKED
float ccdSum=0;
float time,timer;
float t1,tr1,t2,tr2,t3,tr3,tr4,t4,tr5,t5;
unsigned char temp,temp1,temp2=0,temp3=0;
void interrupt 67 PIT1(void)
{

   int i,j;
   
   int tempL,tempR;
   int tempEdge;
   int tempCCD;
   float tempPoint,tempSpeed;
   time=micros();
   PITTF_PTF0 = 1; //clear interrupts flag
   PORTA_PA7=1;
   
   //reading switches signal
   temp=PTH;
    StopCarOn=temp&0b00000001;
    //temp1=temp&0b00000111;
    //SPEED_CONTROL_P=temp1*12;
    temp1=temp>>3&0b00000111;
    if(temp1==0)
     SetSpeedMM=0;
    else
      SetSpeedMM=-1400+temp1*200*-1;  //steady state :1800
    fspeed=SetSpeedMM;
    //setSpeed(SetSpeedMM);
    temp1=temp>>6&0b0000001;
    if(temp1!=temp2) {
      OriginPoint-=1;
      temp2=temp1;
    }
    temp1=temp>>7&0b0000001;
    if(temp1!=temp3) {
      OriginPoint+=1;
      temp3=temp1;
    }
  
  
  temp=PTH;
    temp1=temp>>1&0b00000001; //accelerate
  
  //Determin what to do according to the switch state.
  switch(temp1){
    
    case 0:
            tempPoint=OriginPoint;
            break;
    case 1:
            if(abs(LineCenter-64)<8) {
            tempSpeed= g_fCarSpeed*MMperPulse*1000/(5*scPeriod);
             if(tempSpeed-900>SetSpeedMM)
                tempPoint=OriginPoint+5;
             else if(tempSpeed-800>SetSpeedMM)
                tempPoint=OriginPoint+4;
             else if(tempSpeed-700>SetSpeedMM)
                tempPoint=OriginPoint+3;
             else if(tempSpeed-600>SetSpeedMM)
                tempPoint=OriginPoint+2;
             else if(tempSpeed-500>SetSpeedMM)
                tempPoint=OriginPoint+1;
             else if(tempSpeed-400>SetSpeedMM)
                tempPoint=OriginPoint+0.5;
             else
                tempPoint=OriginPoint;
            }
            else {
              if(tempSpeed<SetSpeedMM)
                tempPoint=OriginPoint-1;
              else
                tempPoint=OriginPoint;
            }
            break;
  }
  
  //Decide whether we're going to deal with obstacles or not.
  temp1=temp>>2&0b00000001;
  if(temp1){
      
    if(obstacleSign==true) {
     obstacleCounter++;
    if(obstacleCounter>=100) {
      obstacleCounter=0;
     obstacleSign=false;
    }
    if(tempSpeed<-2500)
     tempPoint=OriginPoint-4;
    else if(tempSpeed<-2200)
     tempPoint=OriginPoint-3;
    else if(tempSpeed<-2000)
     tempPoint=OriginPoint-2;
    else if(tempSpeed<-1800)
     tempPoint=OriginPoint-1;  
    }
  }
  
  if(lastSwith!=temp) {
    swithChange=true;
    Setpoint=OriginPoint;
    setSpeed(-50);
    scratchLineCount=0;
    scratchLineTimer=millis();
    scratchLine=false;
  } 
  if(swithChange==false){
    if(abs(LineCenter-64)>12)
     SetSpeedMM=-1800; 
    Setpoint=tempPoint;
    setSpeed(SetSpeedMM);
  }
  lastSwith=temp;
  
  
  t1=micros();
  
   CCDTime++;
   if(CCDTime>=ccdMultiple) {
     CCDTime=0; 
     RD_CCD(0);   
     ADV[0][0]=ADV[0][1];//adv[0][0] is a bad point
     CalculateCCD0();
   
   }


   
   getAD ();
   AngleCalculate();
   tr2=micros()-t2;
   
   t3=micros();
   getspeed();
   Input=angleFilter2;
   PID();
   g_nSpeedControlPeriod ++;
   SpeedControlOutput();  
   DirectionControlOutput();
   g_nSpeedControlCount ++;
   if(g_nSpeedControlCount >=scPeriod) 
   { 
        SpeedControl();
        g_nSpeedControlCount = 0;
        g_nSpeedControlPeriod = 0;        
    }
    
    g_nDirectionControlCount++;   
      if(g_nDirectionControlCount >=dcPeriod) 
      {
        DirectionControl();
        g_nDirectionControlCount = 0;
        g_nDirectionControlPeriod = 0;
      }
    SpeedOutCalculate ();   
   //LOUT=-(Output);
   //ROUT=-(Output);
   speedout ();   
   tr3=micros()-t3;
   
   timer=micros()-time;
}

#pragma CODE_SEG DEFAULT         

