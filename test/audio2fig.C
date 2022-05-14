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

#include <string.h>

#include "../Audio/audio.H"
#include "XFigOOutput.H"
//#include "XFigParse.H"

#define TCT TimeCode<MasterCounter<field, 4>, short int>

int main (int argc, char *argv[]){
  //Param fixing
  if (argc!=3){
    cout<<"Useage:\n\t"<<argv[0]<<" objectComment filename.wav"<<endl;
    cout<<"Input : objectComment - {wline=waveformLine, fline=frequencyLine, vline=volumeLine, etc}\n\tfilename.wav - wav file to convert to fig file - MUST BE MONO"<<endl;
    cout<<"Output: filename.wav.fig - fig file representation of the wav file"<<endl;
    exit(-1);
  }

  //Init an audio structure
  Audio<TCT> inputAudio;
  // Open an audio file
  inputAudio.openFile(argv[2]);

  // Display its timeCode (size)
  cout<<*inputAudio.timeCode<<endl;

  //Parse the XFig header
  // a] re-aligning and scaling to the desired visual size/offset in the fig canvas
  XFigOOutput output(strcat(argv[2],".fig"), inputAudio.timeCode->getEnd(), argv[1]);

  // b] output to the fig file
  int cnt=0;
  //  while ((inputAudio.timeCode->getCount())<(inputAudio.timeCode->getEnd())){
  while (cnt<inputAudio.timeCode->getEnd()){
    //    audio.readAudio();
    inputAudio.readAudioNoGui();
    for (int j=0;j<inputAudio.timeCode->window->getCount();j++){
      output<<4*(cnt+j)<<' '<<-(*inputAudio.timeCode->window)[j]<<' ';
      if (((cnt+j+1)%6)==0 && j!=(inputAudio.timeCode->window->getCount()-1))
	output<<"\n\t";
    }
    (*inputAudio.timeCode+=inputAudio.timeCode->window->getCount());
    cnt+=inputAudio.timeCode->window->getCount();
  }
  output.close();
}
