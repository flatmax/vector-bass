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

#ifndef WAVEFORM_H_
#define WAVEFORM_H_
#include <fstream>
#include <math.h>
#include "LinkList.H"
#include "Location.H"

#define SAMPLE_RATE 44100

#ifndef FROUND
#define FROUND
inline double fround(double x){
  if ((x-floor(x))>=0.5) return ceil(x); else return floor(x);
}
#endif

#define _2PI 6.2831853071795862319959269371

class WaveForm {
  double *wave;
  int sCount;
public:
  WaveForm(LinkList<double> *b, LinkList<double> *c, LinkList<loc *> *locs){
    wave=NULL;
    sCount=SAMPLE_RATE*2;
    if (b->getCount()>sCount){
      cout<<"WaveForm::WaveForm : Expanding sample count"<<endl;
      sCount=b->getCount();
    }

    if (!(wave=new double[sCount])){
      cerr<<"WaveForm::WaveForm : wave malloc error"<<endl;
      exit(-1);
    }
    //procure the wave form ....
    locs->grab(1); b->grab(1); c->grab(1);
    double minX, maxX, lastX, t;
    double width=(maxX=locs->prev()->x), factor=1.0/(double)sCount*width;
    width-=(lastX=minX=locs->next()->x);
    double lastJ=0.0, endJ;
    locs->next();
    //cout<<"lastX, prevX, width "<<lastX<<'\t'<<maxX<<'\t'<<width<<endl;
    //ofstream output("temp.txt");
    for (int i=0;i<b->getCount();i++){
      //cout<<"x-lastX/width : "<<locs->current()->x<<' '<<lastX<<' '<<width<<' '<<sCount<<' '<<(((locs->current()->x-lastX)/width)*sCount)<<endl;
      endJ=lastJ+((locs->current()->x-lastX)/width)*sCount;
      int j;
      double grad=b->current(), cStart=c->current();
      //cout<<i<<": b, c "<<grad<<'\t'<<cStart<<endl;
      //cout<<"lastJ, endJ "<<lastJ<<'\t'<<endJ<<endl;
      t=0.0;
      for (j=(int)fround(lastJ);j<(int)fround(endJ);j++){
	t+=factor;
	wave[j]=grad*t+cStart;
	//output<<wave[j]<<endl;
	//output<<t<<endl;
      }
      lastJ=endJ;
      lastX=locs->current()->x;
      locs->next(); b->next(); c->next();
    }
    //      output.close();
  }

  ~WaveForm(void){
    if (wave) delete [] wave;
  }

  //returns the element matching the radians provided ..
  double operator[](double rads){
    //int which=(int)fround(fmod(rads,_2PI)/_2PI*sCount);
    //    cout<<"found rads, which "<<rads<<'\t'<<which<<endl;
    //cout<<fmod(rads,_2PI)<<endl;
    return wave[(int)fround(fmod(rads,_2PI)/_2PI*sCount)];
  }
};
#endif //WAVEFORM_H_
