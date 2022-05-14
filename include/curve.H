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

#ifndef CURVE_H_
#define CURVE_H_
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

/** Specifies straight line segments of the form y=bx+c
 */
class curve {
public:
  LinkList<double> b;
  LinkList<double> c;
  int sCount;

  ~curve(void){
    while (b.getCount())
      b.remove();
    while (c.getCount())
      c.remove();
  }

  void init(LinkList<loc *> *locs){
    while (b.getCount())
      b.remove();
    while (c.getCount())
      c.remove();
    double lastX=locs->grab(1)->x;
    double lastY=locs->current()->y;
    double maxX=locs->prev()->x;
    double minX=locs->next()->x;
    sCount=(int)fround(maxX-minX);
    for (int i=0;i<locs->getCount()-1;i++){
      locs->next();
      //      b.add((locs->current()->y-lastY)/(locs->current()->x-lastX)/(maxX-minX));
      b.add((locs->current()->y-lastY)/(locs->current()->x-lastX));
      c.add(lastY);

      //      cout<<"x, lastX sCount "<<locs->current()->x<<'\t'<<lastX<<'\t'<<sCount<<endl;

      lastX=locs->current()->x;
      lastY=locs->current()->y;
    }
  }
};
#endif //CURVE_H_




