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

/* A note on history of conception for MFFM VectorSynth (previously VectorBass) .....
   Originaly this was based on using the ERB scal for perceptual scaling,
   something similar but more accurate then the Bark band scale. ERB
   references are now historic.
 */

#ifndef VECTORSYNTH_H_
#define VECTORSYNTH_H_
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

class VectorSynth {
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
  int generateSynth(void);

  enum VECTORSYNTH_ERRORS {FILEOPEN_ERR=-1, FERQ_ERR=-2, TIME_ERR=-3, RANGE_ERR=-4, ERBRANGE_ERR=-5, FERQERB_ERR=-6, MALLOC_ERR=-7, XALIGNED_ERR=-7};

  BassLine *outputAudio;
  int changeOver;//Used to indicate new audio is ready for changeOver

  VectorSynth(void);
  virtual ~VectorSynth(void);

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
  @param yi The minimum ERB range line y value
  @param ya The maximum ERB range line y value
  */
  void addFreqRange(double yi, double ya){
    ERBRange.add(new loc(0.,yi,0.));
    ERBRange.add(new loc(0.,ya,0.));
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
  @param yi The minimum wav range line y value
  @param ya The maximum wav range line y value
  */
  void addWavRange(double yi, double ya){
    WavRange.add(new loc(0.,yi,0.));
    WavRange.add(new loc(0.,ya,0.));
  }

  /** Add a point on the waveform line which is the waveform to play at
  this time (x) location.
  @param x The time location relative to all other Wave x locations
  @param y The waveform location >yi and < ya
  */
  void addWavPoint(double x, double y){Wave.add(new loc(x,y,0.));}

  /** Get the numer of samples in the output audio
  @return the number of samples in the output audio file
  */
  int getN(){return outputAudio->audioSampleCount;}

  /** Get the audio from the synth line
  Throws error if sample n doesn't exist
  @param n Get sample number n
  @returns Fill the sample in this variable
  */
  VBATYPE getSample(int n){
    if (n<0 || n>=getN())
      throw invalid_argument("n is out of range - getN() gives maximum N, 0<=n<getN()");
    return outputAudio->audio[n];
  }
};
#endif //VECTORSYNTH_H_
