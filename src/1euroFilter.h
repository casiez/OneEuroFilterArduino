/* -*- coding: utf-8 -*-
 *
 * 1euroFilter.h -
 *
 * Authors: 
 * Nicolas Roussel (nicolas.roussel@inria.fr)
 * Géry Casiez https://gery.casiez.net
 *
 * Copyright 2019 Inria
 * 
 * BSD License https://opensource.org/licenses/BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice, this list of conditions
 * and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions
 * and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or
 * promote products derived from this software without specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <inttypes.h>

#ifndef ONEEUROFILTER
#define ONEEUROFILTER

// -----------------------------------------------------------------
// Utilities

typedef float TimeStamp ; // in seconds

static const TimeStamp UndefinedTime = -1.0 ;

// -----------------------------------------------------------------

#define DEFAULT_FREQUENCY   120   // [Hz] default frequency of your incoming noisy data
#define DEFAULT_MINCUTOFF   1.0   // [Hz] needs to be tuned
#define DEFAULT_BETA        0   // Needs to be tuned
#define DEFAULT_DCUTOFF     1.0   // [Hz] should not be changed

class LowPassFilter {
    
  float y, a, s ;
  bool initialized ;

  void setAlpha(float alpha);

public:

  LowPassFilter(float alpha, float initval=0.0) ;

  float filter(float value) ;

  float filterWithAlpha(float value, float alpha) ;

  bool hasLastRawValue(void) ;

  float lastRawValue(void) ;

  float lastFilteredValue(void) ;

} ;

// -----------------------------------------------------------------

class OneEuroFilter {

  float freq ;
  float mincutoff ;
  float beta_ ;
  float dcutoff ;
  LowPassFilter *x ;
  LowPassFilter *dx ;
  TimeStamp lasttime ;

  // implicitly implemented, not to be used
  OneEuroFilter(const OneEuroFilter&);
  OneEuroFilter& operator=(const OneEuroFilter&);

  float alpha(float cutoff) ;

  void setDerivateCutoff(float dc) ;

public:

  explicit OneEuroFilter(float freq = DEFAULT_FREQUENCY, 
		float mincutoff=DEFAULT_MINCUTOFF, float beta_=DEFAULT_BETA, float dcutoff=DEFAULT_DCUTOFF) ;

  void begin(float freq, 
		float mincutoff=1.0, float beta_=0.0, float dcutoff=1.0) ;

  void setFrequency(float f) ;

  void setMinCutoff(float mc) ;

  void setBeta(float b) ;

  float filter(float value, TimeStamp timestamp=UndefinedTime) ;

  ~OneEuroFilter(void) ;

} ;

#endif