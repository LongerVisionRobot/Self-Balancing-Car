/*
Copyright (c) <2013-2016> <Malcolm Ma>

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
#include <hidef.h>             /* common defines and macros */
#include "derivative.h"        /* derivative-specific definitions */
#include "times"
#include "macros.h"
#include "ADC.h"

//============= Definition and parameters of Linear CCD Sensor==============
int16_t ccdMultiple=1,
int16_t CCDBL=3;
uint8_t CCDRAW[2][CCD_PIXELS] = { 0 };     //Buffer to store the raw CCD sampling data
uint8_t CCDLine[CCD_PIXELS] = { 0 };
int16_t CCDDebugSwitch = 0;
int16_t CCDDebugSwitch2 = 2;
float thresholdCoef = 0.5;                //0.35
uint8_t ObstacleSign=false;

static inline void setCLKLow(const uint16_t time){
  TSL_CLK=LOW;    //Set the clk to low
  delayUs(time);  //delay for 1us
}
static inline void setCLKHigh(const uint16_t time){
  TSL_CLK=LOW;    //Set the clk to low
  delayUs(time);  //delay for 1us
}

/*
 * Before sampling the ADC value of the CCD sensor,we must
 * set the SI signal to inform the CCD sensor to get ready.
 * See datasheet for timing detail.
 */
static void startReadingCCD(){
    TSL_SI=LOW;     
    setCLKLow(1);
    
    TSL_SI=HIGH;     
    delayUs(1);   
    
    TSL_SI=LOW;     
    setCLKHigh(1);
}

/*
 * Read the CCD sensor.Currently there are at most only two sensors on
 * the car.
 */
void readCCD(uint16_t num){
  DisableInterrupts;
  startReadingCCD();
  for(int i=0;i<CCD_PIXELS;i++){
    setCLKLow(1);
    ADV[num][i]=ADC_Read(num);
    setCLKHigh(1);
  }
  EnableInterrupts;
}

int16_t calculateCCD() {
  if (CCDDebugSwitch2 == 1) {           //Enable median value filter
    for (int i = 1; i < 127; i++) {
      CCDSendingBuffer[i] = mid(&CCDRAW[0][i - 1]);
      CCDData[i] = CCDSendingBuffer[i];
    }
  } else {
    for (int i = 0; i < CCD_PIXELS; i++) {
      CCDSendingBuffer[i] = CCDRAW[0][i];
      CCDData[i] = CCDRAW[0][i];
    }
  }

  int16_t averageValue = 0;
  int16_t threshold;
  
  //Calculate average value
  for (int i = 0; i < CCD_PIXELS; i++) {     
    averageValue += CCDData[i];
  }
  averageValue = averageValue / CCD_PIXELS;
  threshold = averageValue * thresholdCoef;

  //Determin if there is an obstacle ahead.An obstacle is a black stripe.
  if (angleFinal > OriginPoint - 10 && angleFinal < OriginPoint + 15) //-74~-49
    if (averageValue < Threshold * 2 / 3)
      ObstacleSign = true;

  //Now count how many edges are detected,we use the number of edges to determin
  //if the car has reach the end line.
  int16_t edgeCount=0;
  int16_t tempEdge;
  for (int16_t i = 0, tempEdge = CCD_PIXELS; i + STEP < CCD_PIXELS; i++) {
    if (abs(CCDData[i] - CCDData[i + STEP]) > threshold) {
      if (abs(tempEdge - i) > STEP * 2) {
        tempEdge = i;
        edgeCount++;
      }
    }
  }
  //After knowing the number of edges,we now determine if the car reaches the end line.
  //
  if (edgeCount > 5 && (millis() - startTime) > 10000) {
    ReachedEnd = true;
  }

  //Now calculating the center line,the basic method is to measure the left and right
  //edge and then divide by two.So first we search for the black line to the left and right.
  int16_t blackLineR, blackLineL;
  int16_t lineCenter = CCD_PIXELS/2;
  for (int16_t i = lineCenter; i + STEP < CCD_PIXELS; i++) {     //Finds the black line to the righ
    if (CCDData[i] - CCDData[i + STEP] > threshold) {
      blackLineR = i + STEP;                              //Found it
      //Set the right part of the black line to all black
      for (i = blackLineR; i < CCD_PIXELS; i++) {            
        CCDData[i] = 0;
      }
      break;
    }
  }
  for (int16_t i = lineCenter; i - STEP >= 0; i--) {          //Finds the black line to the left
    if (CCDData[i] - CCDData[i - STEP] > threshold) {
      blackLineL = i - STEP;                              //Found it
      //Set the left part of the black line to all black
      for (i = blackLineL; i >= 0; i--) {
        CCDData[i] = 0;
      }
      break
    }
  }

  //Calculate the width of the race track,the predicate is to ensure that
  //we only update the track width data only if the car detects black lines
  //on both side.
  if (blackLineL > STEP && blackLineR < CCD_PIXELS-STEP)
    trackWidth = blackLineR - blackLineL;

  //If only left side is detected then we use the track width information
  //to calculate the center line.Same as when there's only right line.
  if (blackLineL > 50)
    lineCenter = blackLineL + trackWidth / 2;
  else if (blackLineR < 78)
    lineCenter = blackLineR - trackWidth / 2;
  else
    lineCenter = (blackLineL + blackLineR) / 2;

  //Constrain the result
  if (lineCenter + STEP > CCD_PIXELS)
    lineCenter = CCD_PIXELS;
  else if (lineCenter < STEP)
    lineCenter = STEP;

  //Use low pass FIR filter to smooth the change
  lineCenter = FIR(3, lineCenter);
  return lineCenter;
}
