/* Copyright 1998, 1999, 2000, 2001 Matt Flax <flatmax@ieee.org>
   This file is part of MFFM VectorBass.

   MFFM VectorBass is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   MFFM VectorBass is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You have received a copy of the GNU General Public License
   along with MFFM VectorBass
 */

/* A note on history of conception for MFFM VectorBass .....
   Originaly this was based on using the ERB scal for perceptual scaling,
   something similar but more accurate then the Bark band scale. ERB 
   references are now historic.
 */

#ifndef VECTORBASS_H_
#define VECTORBASS_H_
#include <fstream.h>
#include <math.h>
#include <LinkList.H>
#include "curve.H"
#include "waveForm.H"

#define SAMPLE_RATE 44100
#define VBATYPE short int
#define OCTAVE 12

#define OUTPUTLEVEL (pow(2,sizeof(VBATYPE)*8-1)-1.0)
#define ALPHA 0.8

#define XMAX 100
#define YMAX 10

#ifndef FROUND
#define FROUND
inline double fround(double x){
  if ((x-floor(x))>=0.5) return ceil(x); else return floor(x);
}
#endif

class BassLine {
public:
  VBATYPE *audio;
  int audioSampleCount;

  BassLine(int size){
    audio=NULL;
    audioSampleCount=size;
    if (!(audio=new VBATYPE[audioSampleCount])){
      cerr<<"BassLine::BassLine : Couldn't alloc array of size "<<audioSampleCount<<endl;
    }
  }

  ~BassLine(){
    if (audio) delete [] audio; audio=NULL;
    audioSampleCount=0;
  }

  /*  VBATYPE& operator[](int i){
    if (i<audioSampleCount)
      return array[i];
    else
      return array[0];
      }*/
};

class VectorBass {
  LinkList<double> Freq;
  LinkList<double> ERB;
  double time;
  double range;
  LinkList<loc *> ERBRange;
  LinkList<loc *> FreqERB;
  LinkList<loc *> Volume;
  curve VolCurve;

  LinkList<loc *> WavRange;
  LinkList<loc *> Wave;
  curve WaveCurve;
  WaveForm *waveForm;

  double xMax, xMin,yMax, yMin;
  //  double *splineInX, *splineInY;
  //  real *splineOutX, *splineOutY;
  double *procAudio;

  int allocMem(void);
  void deAllocMem(void);
  int generateBass(void);
  int readFile(const char *fName);
  //int readFile(int fID);
  int parseFig(ifstream *input);

  BassLine *audio; // The audio which is being processed
protected:
  double getTime(){return time;}
public:
  enum VECTORBASS_ERRORS {FILEOPEN_ERR=-1, FERQ_ERR=-2, TIME_ERR=-3, RANGE_ERR=-4, ERBRANGE_ERR=-5, FERQERB_ERR=-6, MALLOC_ERR=-7, XALIGNED_ERR=-7};

  BassLine *outputAudio;
  int changeOver;//Used to indicate new audio is ready for changeOver

  VectorBass(void);
  virtual ~VectorBass(void);

  int writeFile(const char *fName);
  int writeWavFile(const char *fName);
 
  void goChangeOver();
  int processFile(const char *iFName);
  //  int processFile(int iFID);
};
#endif //VECTORBASS_H_
