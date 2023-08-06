/*
 * demo.ino -
 *
 * Authors: 
 * Géry Casiez https://gery.casiez.net
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

static OneEuroFilter f; // not enabled yet, setup has to be called later

// Frequency of your incoming noisy data
// If you are able to provide timestamps, the frequency is automatically determined
#define FREQUENCY   60   // [Hz] 
#define MINCUTOFF   3.0   // [Hz] needs to be tuned according to your application
#define BETA        0.1   // needs to be tuned according to your application

unsigned long start_time;

void setup() {
  // setup filter
  f.begin(FREQUENCY, MINCUTOFF, BETA);

  // initialize serial port
  Serial.begin(9600);

  start_time = micros();
}

void loop() {
  float elapsed_time = 1E-6 * (micros() - start_time); // in seconds
  float clean_signal = 10 * sin(elapsed_time * 6);
  
  long noise = random (-10, 10);
  float noisy_signal = clean_signal + noise / 10.0;

  float filtered_signal = f.filter(noisy_signal, elapsed_time) ;
  
  // The second parameter with timestamps can be omitted as following (FREQUENCY is then used)
  // float filtered_signal = f.filter(noisy_signal);

  // Frequency can also be adjusted at a later stage using setFrequency method
  // f.setFrequency(120);

  // MinCutoff can be adjusted using setMinCutoff method
  // For example a potentiometer can be connected to the arduino to adjust in real time its value
  // f.setMinCutoff(0.1);

  // Beta can be adjusted using setBeta method
  // For example a potentiometer can be connected to the arduino to adjust in real time its value
  // f.setBeta(0.001);

  // Printing data according to the format expected by Serial Plotter (go in Tools > Serial Plotter)
  Serial.print("Clean_signal:");
  Serial.print(clean_signal,3);
  Serial.print(",");
  Serial.print("noisy_signal:");
  Serial.print(noisy_signal,3);
  Serial.print(",");
  Serial.print("filtered_signal:");
  Serial.println(filtered_signal,3);

  // Pause during 15 ms (~60 Hz)
  delay(15);
}
