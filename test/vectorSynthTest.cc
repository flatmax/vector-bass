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
  if (argc != 3){
    cerr<<"Useage: "<<arg[0]<<" filename.fig otuput.wav"<<endl;
    exit(-1);
  }

  VectorSynth vb;
  vb.processFile(arg[1]);
  while (!vb.changeOver);
  vb.goChangeOver();

#ifdef HAVE_LIBSNDFILE
  vb.writeWavFile(arg[2]);
#else
  cout<<"\n\nWARNING : libsndfile not detected on yoru system at build time, not saving the wavfile\n\n"<<endl;
#endif
  return 1;
}
