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

#include <LinkList.H>

#include "XFigParse.H"
#include "../2daudio/Location.H"

/** Compile and run like so :
    g++ XFigParseTest.cc -o XFigParseTest -L/home/flatmax/lib -l3DPanner; ./XFigParseTest hello.fig
*/

LinkList<double> Freq;
double time;
double range;
LinkList<loc *> ERBRange;
LinkList<loc *> FreqERB;
LinkList<loc *> Volume;
LinkList<loc *> WavRange;
LinkList<loc *> Wave;

void main(int argc, char **arg){
  //for (int i=0;i<argc;i++)
  //  cout<<arg[i]<<endl;

  if (argc != 2){
    cerr<<"Useage: "<<arg[0]<<" filtename.fig"<<endl;
    exit(-1);
  }

  ifstream input(arg[1]);
  XFigParse parser;
  parser.getVariables(&Freq, &time, &range, &ERBRange, &FreqERB, &Volume, &WavRange, &Wave, &input);
}
