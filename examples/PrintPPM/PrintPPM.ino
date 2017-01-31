/*
  Copyright (c) 2017 TechTown Foundation (http://gotechtown.org)

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

  Written by Aram Perez
*/

#include <PPM_Decoder.h>

#define PPM_PIN A0
#define NUM_OF_CHANNELS 6
#define NUM_OF_SAMPLES 1
#define SYNC_TIME 8000

PPM_Decoder decoder(PPM_PIN, NUM_OF_CHANNELS, NUM_OF_SAMPLES, SYNC_TIME);

void setup()
{
  Serial.begin(115200);
  Serial.print("\r\nTesting PPM decoder...");
  if ( decoder.isValid() ) {
    Serial.println("ok.");
  } else {
    Serial.println("\r\nError: Decoder is not valid, halting.");
    while(true);
  }
}

void print5Wide(unsigned v, char* pSeparator = NULL)
{
  if ( v < 10 ) {
    Serial.print("0000");
  } else if ( v < 100 ) {
    Serial.print("000");
  } else if ( v < 1000 ) {
    Serial.print("00");
  } else if ( v < 10000 ) {
    Serial.print("0");
  }
  Serial.print(v);
  if ( pSeparator ) {
    Serial.print(pSeparator);
  }
}

void loop()
{
  decoder.update();
  for ( int i = 0; i < NUM_OF_CHANNELS; i++ ) { //Loop to print and clear all the channel readings
    print5Wide(decoder.value(i), " ");
  }
  Serial.println(); //Start a new line
}
