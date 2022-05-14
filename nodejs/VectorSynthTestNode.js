#! /usr/bin/env node
// Copyright (c) 2017 The WASM Authors. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//    * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//    * Neither the name of mad chops coder AU nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

"use strict;"

let libVS = require('./libVSNode.js');
libVS().then((mod)=>{
  libVS = mod;
  let vs = new libVS.VectorSynth;

  vs.addFreq(330.); // can add more then  one
  vs.setTime(5.); // duration in seconds
  vs.setRange(24.); // Number of piano semitones

  vs.addFreqRange(990., 900., 225., 3600.); // y axis over which the setRange semitones exist

  // add some pitch variation curves
  vs.addFreqPoint(1800., 4140.);
  vs.addFreqPoint(1369., 2956.);
  vs.addFreqPoint(2045., 2710.);
  vs.addFreqPoint(2477., 3894.);
  vs.addFreqPoint(2976., 2371.);

  // volume changes
  vs.addVolPoint(1800., 901.);
  vs.addVolPoint(2250., 900.);
  vs.addVolPoint(2700., 900.);
  vs.addVolPoint(3150., 900.);
  vs.addVolPoint(3600., 900.);
  vs.addVolPoint(4050., 900.);
  vs.addVolPoint(4500., 900.);
  vs.addVolPoint(4950., 900.);
  vs.addVolPoint(5400., 900.);
  vs.addVolPoint(6255., 899.);

  // waveform y range for max amplitude and min amplitude
  vs.addWavRange(225., 6750., 225., 8100.);

  // define the waveform to oscillate
  vs.addWavPoint(360., 7425.);
  vs.addWavPoint(630., 6750.);
  vs.addWavPoint(1035., 8100.);
  vs.addWavPoint(1260., 7425.);

  let ret;
  if ((ret=vs.process())!=0)
    return ret;
  if ((ret=vs.generateSynth())<0)
    return ret;

  vs.goChangeOver();

  // #ifdef HAVE_LIBSNDFILE
  //   vs.writeWavFile(arg[1]);
  // #else
    console.log("\n\nWARNING : libsndfile not detected on your system at build time, not saving the wavfile\n\n");
  // #endif
  // return 1;
});
