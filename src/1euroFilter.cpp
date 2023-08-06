/* -*- coding: utf-8 -*-
 *
 * 1euroFilter.cpp -
 *
 * Authors: 
 * Géry Casiez https://gery.casiez.net
 * Nicolas Roussel (nicolas.roussel@inria.fr)
 *
 * Copyright 2023 Inria
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

#include "1euroFilter.h"

// -----------------------------------------------------------------



  void LowPassFilter::setAlpha(float alpha) {
    a = alpha ;
  }

  LowPassFilter::LowPassFilter(float alpha, float initval) {
    y = s = initval ;
    setAlpha(alpha) ;
    initialized = false ;
  }

  float LowPassFilter::filter(float value) {
    float result ;
    if (initialized)
      result = a*value + (1.0-a)*s ;
    else {
      result = value ;
      initialized = true ;
    }
    y = value ;
    s = result ;
    return result ;
  }

  float LowPassFilter::filterWithAlpha(float value, float alpha) {
    setAlpha(alpha) ;
    return filter(value) ;
  }

  bool LowPassFilter::hasLastRawValue(void) {
    return initialized ;
  }

  float LowPassFilter::lastRawValue(void) {
    return y ;
  }

  float LowPassFilter::lastFilteredValue(void) {
    return s ;
  }


// -----------------------------------------------------------------


  float OneEuroFilter::alpha(float cutoff) {
    float te = 1.0 / freq ;
    float tau = 1.0 / (2*M_PI*cutoff) ;
    return 1.0 / (1.0 + tau/te) ;
  }

  void OneEuroFilter::setFrequency(float f) {
    freq = f ;
  }

  void OneEuroFilter::setMinCutoff(float mc) {
    mincutoff = mc ;
  }

  void OneEuroFilter::setBeta(float b) {
    beta_ = b ;
  }

  void OneEuroFilter::setDerivateCutoff(float dc) {
    dcutoff = dc ;
  }

  OneEuroFilter::OneEuroFilter(float freq, 
		float mincutoff, float beta_, float dcutoff) {
      begin(freq, mincutoff, beta_, dcutoff);
  }

  void OneEuroFilter::begin(float freq, 
		float mincutoff, float beta_, float dcutoff) {
    setFrequency(freq) ;
    setMinCutoff(mincutoff) ;
    setBeta(beta_) ;
    setDerivateCutoff(dcutoff) ;
    x = new LowPassFilter(alpha(mincutoff)) ;
    dx = new LowPassFilter(alpha(dcutoff)) ;
    lasttime = UndefinedTime ;
  }

  float OneEuroFilter::filter(float value, TimeStamp timestamp) {
    // update the sampling frequency based on timestamps
    if (lasttime!=UndefinedTime && timestamp!=UndefinedTime)
      freq = 1.0 / (timestamp-lasttime) ;
    lasttime = timestamp ;
    // estimate the current variation per second 
    float dvalue = x->hasLastRawValue() ? (value - x->lastFilteredValue())*freq : 0.0 ; // FIXME: 0.0 or value?
    float edvalue = dx->filterWithAlpha(dvalue, alpha(dcutoff)) ;
    // use it to update the cutoff frequency
    float cutoff = mincutoff + beta_*fabs(edvalue) ;
    // filter the given value
    return x->filterWithAlpha(value, alpha(cutoff)) ;
  }

  OneEuroFilter::~OneEuroFilter(void) {
    delete x ;
    delete dx ;
  }

