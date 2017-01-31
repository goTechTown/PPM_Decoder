#ifndef PPM_DECODER_H
#define PPM_DECODER_H
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

#include <Arduino.h>
#include <limits.h>

class PPM_Decoder {
	uint8_t pin;
	unsigned nbrOfChannels;
	unsigned nbrOfSamples;
	unsigned long syncTime;
	unsigned* pValues;

	PPM_Decoder();

public:
	PPM_Decoder(uint8_t pinNbr, unsigned nbrOfChs, unsigned nbrOfSams = 1, unsigned long syncTimeMs = 5000) :
		pin(pinNbr), nbrOfChannels(nbrOfChs), nbrOfSamples(nbrOfSams), syncTime(syncTimeMs) {
		pinMode(pin, INPUT_PULLUP);
		pValues = new unsigned[nbrOfChs * nbrOfSams];
	}
	virtual ~PPM_Decoder() {
		if ( pValues != NULL ) {
			delete[] pValues;
		}
	}
	inline bool isValid() const {
		return pValues != NULL;
	}
	void update() {
		unsigned* pVals = pValues;
		for ( unsigned j = 0; j < nbrOfSamples; j++ ) {
			while ( pulseIn(pin, LOW) < syncTime) {
				digitalWrite(pin, !digitalRead(pin));
			}
			for ( unsigned i = 0; i < nbrOfChannels; i++ ) {
				*pVals++ = (unsigned) pulseIn(pin, LOW);
			}
		}
	}
	unsigned value(unsigned ch) const {
		if ( ch >= nbrOfChannels ) {
			return UINT_MAX;
		}
		if ( nbrOfSamples == 1) {
			return pValues[ch];
		}
		unsigned long total = 0;
		unsigned* pVal = &pValues[ch];
		for ( unsigned i = 0; i < nbrOfSamples; i++ ) {
			total += *pVal;
			pVal += nbrOfChannels;
		}
		return total / nbrOfSamples;
	}
};

#endif  //PPM_DECODER_H
