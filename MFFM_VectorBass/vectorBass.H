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
#include <fstream>
#include <math.h>
#include "LinkList.H"
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

//#define C1 24.673
#define C2 4.368
//C3=(2302.6/(c1 * c2));
#define C3 21.366

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
  int readFile(const char *fName);
  //int readFile(int fID);
  int parseFig(ifstream *input);

  BassLine *audio; // The audio which is being processed
protected:
  double getTime(){return time;}

  double
  freq2ERB(double freq){
    return (C3*log10((C2 * freq/1000.0) + 1.0));
  }
  double
  ERB2freq(double erb){
    return 1000.0 * (pow(10.0,(erb/C3)) - 1.0) / C2;
  }

public:
  int generateBass(void);

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

  void dumpLL(LinkList<double> *v){
    v->grab(1); v->prev();
    for (int i=0; i<v->getCount(); i++)
      cout<<v->next()<<", ";
    cout<<endl;
  }

  void dumpLL(LinkList<loc *> *v){
    v->grab(1); v->prev();
    for (int i=0; i<v->getCount(); i++)
      cout<<*v->next()<<", ";
    cout<<endl;
  }

  /// process the output
  int process();

  /** Add an oscillation frequency to the list. Each oscillation frequency will be
  the frequency used to oscillate the waveform by. It is polyphonic.
  @param f A frequency to oscillate the waveform at
  */
  void addFreq(double f){Freq.add(f); ERB.add(freq2ERB(f));}

  /** The duration in seconds to generate the output for.
  @param t Time in seconds
  */
  void setTime(double t){time=t;}

  /** The range specifices how many semitones the maximum range spans for
  @param r the range to set
  */
  void setRange(double r){range=r;}

  /** Add points for the frequency measurement line, which hods range semitones.
  I think xi and xa are not important here only yi and ya
  @param xi The minimum ERB range line x value
  @param yi The minimum ERB range line y value
  @param xa The maximum ERB range line x value
  @param ya The maximum ERB range line y value
  */
  void addFreqRange(double xi, double yi, double xa, double ya){
    ERBRange.add(new loc(xi,yi,0.));
    ERBRange.add(new loc(xa,ya,0.));
  }

  /** Add a point on the frequency range line which is the ferquency to play at
  this time (x) location.
  @param x The time location realtive to all other FreqERB x locations
  @param y The frequency location >yi and < ya
  */
  void addFreqPoint(double x, double y){FreqERB.add(new loc(x,y,0.));}

  /** Add a point on the volume range line which is the volume to play at
  this time (x) location.
  @param x The time location relative to all other Volume x locations
  @param y The volume location >yi and < ya
  */
  void addVolPoint(double x, double y){Volume.add(new loc(x,y,0.));}

  /** Add points for the waveform line, which holds waveform maximum and minimum in y.
  I think xi and xa are not important here only yi and ya
  @param xi The minimum wav range line x value
  @param yi The minimum wav range line y value
  @param xa The maximum wav range line x value
  @param ya The maximum wav range line y value
  */
  void addWavRange(double xi, double yi, double xa, double ya){
    WavRange.add(new loc(xi,yi,0.));
    WavRange.add(new loc(xa,ya,0.));
  }

  /** Add a point on the waveform line which is the waveform to play at
  this time (x) location.
  @param x The time location relative to all other Wave x locations
  @param y The waveform location >yi and < ya
  */
  void addWavPoint(double x, double y){Wave.add(new loc(x,y,0.));}
};
#endif //VECTORBASS_H_
