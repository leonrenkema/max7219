/*
* The MIT License (MIT)
*
* Copyright (c) JEMRF
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
********************************************************************************
*
* Module     : max7219.cpp
* Author     : Jonathan Evans
* Description: MAX7219 LED Display Driver
*
* The MAX7219/MAX7221 are compact, serial input/output common-cathode display drivers that interface
* microprocessors (µPs) to 7-segment numeric LED displays of up to 8 digits, bar-graph displays, or 64 
* individual LEDs
* Datasheet  : https://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf
*
* Library Description
*
*  - This library implements the 7-segment numeric LED display of 8 digits 
*  - The host communicates with the MAX7219 using three signals: CLK (pin 10), CS (pin 11), DIN (pin 12). 
*  - Pins can be configured in max7219.h
*  - The MAX7219 is a SPI interface
*  - This library uses the bitbang method for communication with the MAX7219 
*
* Usage
*
* Three methods are exposed for use:
*  
*  1. Begin
*  This method initializes communication, takes the display out of test mode, clears the screen and sets intensity.
*  Intensity is set at maximum but can be configured in max7219.h
*  
*  2. DisplayChar(Digit, Value, DP)
*  This method displays a single value (character) in position DIGIT (0=right most digit, 7=left most digit)
*  
*  3. DisplayText(Text, Justify)
*  This method displays a text string (Text) either right justified (Justify=0) or left justified (Justify=1) 
*/

#include "max7219.h"

MAX7219:: MAX7219(void)
{
  pinDin = 12;
  pinClk = 10;
  pinCs = 11;

  initPorts();
}

MAX7219::MAX7219(int din, int cs, int clk )
{
  pinDin = din;
  pinCs = cs;
  pinClk = clk;

  initPorts();
}

void MAX7219::initPorts() {
  pinMode(pinDin, OUTPUT);
  pinMode(pinCs, OUTPUT);
  pinMode(pinClk, OUTPUT);
}

void MAX7219::MAX7219_ShutdownStart (void)
{
  MAX7219_Write(REG_SHUTDOWN, 0);                     
}

void MAX7219:: MAX7219_DisplayTestStart (void)
{
  MAX7219_Write(REG_DISPLAY_TEST, 1);                 
}

void MAX7219::Clear(void) {
    
    for(uint8_t i=1;i<9;i++) {
        MAX7219_Write(i,0x00);
    }
}

void MAX7219::MAX7219_DisplayTestStop (void)
{
  MAX7219_Write(REG_DISPLAY_TEST, 0);                 
}

void MAX7219::setBrightness (char brightness)
{                             
  MAX7219_Write(REG_INTENSITY, brightness);           
}

/**
 * @param char 
 * @param bool Decimal point
 */
unsigned char MAX7219::MAX7219_LookupCode (char character, bool dp)
{
  uint8_t i;
  uint8_t d=0;
 if (dp) d=1;  
  if (character>=35 && character<=44) {
    character+=13;
    d=1;
  }
  for (i = 0; MAX7219_Font[i].ascii; i++)              
    if (character == MAX7219_Font[i].ascii){
      if (d){
        d=MAX7219_Font[i].segs;
        d |= (1<<7);
        return (d);                                    
        }
      else{
        return MAX7219_Font[i].segs;                    
      }
    }
      
  return 0;                                             
}

void MAX7219::DisplayText(char *text, bool justify)
{
  bool decimal[16];
  char trimStr[16] = "";
  uint8_t x,y=0;
  uint8_t s;
  
  s=strlen(text);
  if (s>16) s=16;
  for (x=0;x<s;x++){
    if (text[x]=='.'){
      decimal[y-1]=true;
      }
    else{
      trimStr[y]=text[x];
      decimal[y]=false;
      y++;
     }
  }
  if (y>8) y=8;
  for (x=0;x<y;x++) {
      if (justify) {
        DisplayChar((uint8_t)(y-x+7-y),trimStr[x], decimal[x]);    
      } else {
        DisplayChar((uint8_t)(y-x+7-y-(8-y)),trimStr[x], decimal[x]);        
      }
  }
}

void MAX7219::MAX7219_Write(volatile byte opcode, volatile byte data) 
{
  digitalWrite(pinCs, LOW);
  shiftOut(pinDin, pinClk, MSBFIRST, opcode);
  shiftOut(pinDin, pinClk, MSBFIRST, data);
  digitalWrite(pinCs,HIGH);
}    

void MAX7219::DisplayChar(uint8_t digit, char value, bool dp) 
{
      MAX7219_Write(digit+1,MAX7219_LookupCode(value, dp));
}

void MAX7219::MAX7219_ShutdownStop (void)
{
  MAX7219::MAX7219_Write(REG_SHUTDOWN, 1);   
}
 
void MAX7219::Begin()
{ 
  digitalWrite(pinCs,HIGH);
  MAX7219_Write(REG_SCAN_LIMIT, 0x07);                   
  MAX7219_Write(REG_DECODE, 0x00);                    
  MAX7219_ShutdownStop();                             
  MAX7219_DisplayTestStop();                          
  Clear();                                    
  setBrightness(0x04);               
}
