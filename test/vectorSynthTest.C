/* Copyright 1998-2022 Flatmax Pty Ltd
   This file is part of MFFM VectorSynth (previously VectorBass).

   MFFM VectorSynth (previously VectorBass) is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   MFFM VectorSynth (previously VectorBass) is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You have received a copy of the GNU General Public License
   along with MFFM VectorSynth (previously VectorBass)
 */

#include "vectorSynth.H"

int main(int argc, char **arg){
  if (argc != 2){
    cerr<<"Useage: "<<arg[0]<<" output.wav"<<endl;
    exit(-1);
  }

  VectorSynth vs;

  // the following data is taken from newhello.fig

  vs.addFreq(330.); // can add more then  one
  vs.setTime(5.); // duration in seconds
  vs.setRange(24.); // Number of piano semitones

  vs.addFreqRange(900., 3600.); // y axis over which the setRange semitones exist

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
  vs.addWavRange(6750., 8100.);

  // define the waveform to oscillate
  vs.addWavPoint(360., 7425.);
  vs.addWavPoint(630., 6750.);
  vs.addWavPoint(1035., 8100.);
  vs.addWavPoint(1260., 7425.);

  int ret;
  if ((ret=vs.process())!=0)
    return ret;
  if ((ret=vs.generateSynth())<0)
    return ret;

  while (!vs.changeOver);
  vs.goChangeOver();

  #ifdef HAVE_LIBSNDFILE
    vs.writeWavFile(arg[1]);
  #else
    cout<<"\n\nWARNING : libsndfile not detected on your system at build time, not saving the wavfile\n\n"<<endl;
  #endif
  return 1;
}
