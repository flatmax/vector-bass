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

#include <fstream>
#include <sys/stat.h>
#include <values.h>
#include <unistd.h>

#include "config.h"
#ifdef HAVE_LIBSNDFILE
#include "libSndFileWrapper.H"
#endif
#include "vectorSynth.H"
#include "XFigParse.H"

using namespace std;

#define DEG2RAD 2.0*M_PI

VectorSynth::
VectorSynth(void){
  outputAudio=audio=NULL;
  procAudio=NULL;
  waveForm=NULL;
  range=time=0;
  changeOver=0;
}

VectorSynth::
~VectorSynth(void){
  deAllocMem();
  if (outputAudio) delete outputAudio; outputAudio=NULL;
}

void VectorSynth::
deAllocMem(void){
  if (audio) delete audio;
  if (procAudio) delete [] procAudio; procAudio=NULL;
  if (waveForm) delete waveForm; waveForm=NULL;

  time=0;
  while (Freq.getCount()) Freq.remove();
  while (ERB.getCount()) ERB.remove();

  loc *tempLoc;
  while (ERBRange.getCount()){
    tempLoc=ERBRange.remove();
    delete tempLoc;
  }
  while (FreqERB.getCount()){
    tempLoc=FreqERB.remove();
    delete tempLoc;
  }
  while (Volume.getCount()){
    tempLoc=Volume.remove();
    delete tempLoc;
  }
  while (WavRange.getCount()){
    tempLoc=WavRange.remove();
    delete tempLoc;
  }
  while (Wave.getCount()){
    tempLoc=Wave.remove();
    delete tempLoc;
  }
}

int VectorSynth::
allocMem(void){
  int tempCnt=(int)rint(time*(double)SAMPLE_RATE);
  if (tempCnt<=0){
      cerr<<"VectorSynth::allocMem : Size must be > 0 "<<endl;
      return MALLOC_ERR;
  }
  if (!outputAudio)
    if (!(outputAudio=new BassLine(tempCnt))){
      cerr<<"VectorSynth::allocMem : Couldn't alloc outputAudio synthline of size "<<tempCnt<<endl;
      return MALLOC_ERR;
    }

  if (!(audio=new BassLine(tempCnt))){
    cerr<<"VectorSynth::allocMem : Couldn't alloc Audio synthline of size "<<tempCnt<<endl;
    return MALLOC_ERR;
  }
  if (!(procAudio=new double[tempCnt])){
    cerr<<"VectorSynth::allocMem : Couldn't alloc array of size "<<tempCnt<<endl;
    return MALLOC_ERR;
  }

  return 0;
}

int VectorSynth::
readFile(const char *fName){
  //cout<<"VectorSynth::readFile : fName"<<endl;
  // Read in the variables
  ifstream input(fName);
  int res=parseFig(&input);
  if (res==FILEOPEN_ERR)
    cerr<<fName<<endl;
  return res;
}

/*int VectorSynth::
readFile(int fID){
  //cout<<"VectorSynth::readFile : fID"<<endl;
  // Read in the variables
  ifstream input(fID);
  return parseFig(&input);
  }*/

int VectorSynth::
parseFig(ifstream *input){
  cout<<"VectorSynth::parseFig : input"<<endl;
  if (!input){
    cerr<<"VectorSynth::readFile : Couldn't open file "<<endl;
    return FILEOPEN_ERR;
  }
  //Free all memory
  deAllocMem();

  XFigParse xInput;
  xInput.getVariables(&Freq, &time, &range, &ERBRange, &FreqERB, &Volume, &WavRange, &Wave, input);

  return process();
}

int VectorSynth::
process(){
  //Check the variables
  if (Freq.getCount()<1){
    cerr<<"VectorSynth:: : Not enough frequencies "<<endl;
    cout<<"Freq : "<<endl;
    dumpLL(&Freq); // print out this linked list
    return FERQ_ERR;
  }
  if (time<=0){
    cerr<<"VectorSynth:: : Impossible time "<<time<<endl;
    return TIME_ERR;
  }
  if (range<0){
    cerr<<"VectorSynth:: : Impossible range (must be >0)"<<range<<endl;
    return RANGE_ERR;
  }
  if (ERBRange.getCount()!=2){
    cerr<<"VectorSynth:: : Couldn't find the vertical range line "<<endl;
    return ERBRANGE_ERR;
  }
  if (FreqERB.getCount()<2){
    cerr<<"VectorSynth:: : Not enough points in the frequency bend line "<<endl;
    return FERQERB_ERR;
  }

  cout<<"============ BEGIN Dumping variables =================="<<endl;
  cout<<"Freq : "<<endl;
  dumpLL(&Freq); // print out this linked list

  cout<<"time : "<<time<<endl;
  cout<<"range : "<<range<<endl;

  cout<<"ERBRange : "<<endl;
  dumpLL(&ERBRange); // print out this linked list

  cout<<"FreqERB : "<<endl;
  dumpLL(&FreqERB); // print out this linked list

  cout<<"Volume : "<<endl;
  dumpLL(&Volume); // print out this linked list

  cout<<"WavRange : "<<endl;
  dumpLL(&WavRange); // print out this linked list

  cout<<"Wave : "<<endl;
  dumpLL(&Wave); // print out this linked list
  cout<<"============ END Dumping variables =================="<<endl;

  //Alloc memory
  allocMem();
  //Find the ERB values for the frequencies ...
  Freq.grab(1); Freq.prev();
  for (int i=0;i<Freq.getCount();i++)
    ERB.add(freq2ERB(Freq.next()));

  // Re-scale the frerquency points ...
  if (ERBRange.current()->y<ERBRange.next()->y) yMax=ERBRange.next()->y;
  else yMax=ERBRange.current()->y;
  yMin=ERBRange.next()->y;
  xMin=FreqERB.grab(1)->x;
  xMax=FreqERB.prev()->x;
  //cout<<"xMin, xMax : "<<xMin<<'\t'<<xMax<<endl;
  //cout<<"yMin, yMax : "<<yMin<<'\t'<<yMax<<endl;

  //cycle through the freqs shifting and re-scaling as we go
  double height=yMin-yMax, zeroPoint=yMax+height/2.0;
  double length=xMax-xMin;
  double octaveRange=(range/(double)OCTAVE);
  double lastX=-MAXDOUBLE;
  FreqERB.grab(1);
  for (int i=0;i<FreqERB.getCount();i++){
    if ((FreqERB.current()->x-=xMin)<lastX)//Fix X's which are b4 last X
      FreqERB.current()->x=lastX;
    lastX=FreqERB.current()->x;

    FreqERB.current()->y=-octaveRange*(FreqERB.current()->y-zeroPoint)/(height/2.0);

    FreqERB.next();
  }

  yMax=range/(double)OCTAVE; yMin=-range/(double)OCTAVE;

  xMax-=xMin; xMin=0;

  //cout<<"xMin, xMax : "<<xMin<<'\t'<<xMax<<endl;
  //cout<<"yMin, yMax : "<<yMin<<'\t'<<yMax<<endl;

  FreqERB.grab(1);
  for (int i=0;i<FreqERB.getCount();i++){
    cout<<FreqERB.current()->x<<'\t'<<FreqERB.current()->y<<endl;
    FreqERB.next();
  }

  if (WavRange.getCount() && Wave.getCount()){
    // Re-scale the waveform points ...
    double maxX, minX, maxY, minY;
    if (WavRange.current()->y<WavRange.next()->y) maxY=WavRange.next()->y;
    else maxY=WavRange.current()->y;
    minY=WavRange.next()->y;
    minX=Wave.grab(1)->x;
    maxX=Wave.prev()->x;
    //cout<<"minX, maxX : "<<minX<<'\t'<<maxX<<endl;
    //cout<<"minY, maxY : "<<minY<<'\t'<<maxY<<endl;
    //cycle through the freqs shifting and re-scaling as we go
    height=minY-maxY, zeroPoint=maxY+height/2.0;
    length=maxX-minX;
    lastX=-MAXDOUBLE;
    Wave.grab(1);
    for (int i=0;i<Wave.getCount();i++){
      if ((Wave.current()->x-=minX)<lastX)//Fix X's which are b4 last X
	Wave.current()->x=lastX;
      lastX=Wave.current()->x;

      Wave.current()->y=-(Wave.current()->y-zeroPoint)/(height/2.0);

      Wave.next();
    }

    //for (int i=0;i<Wave.getCount();i++)
    //  cout<<*Wave.grab(i+1)<<endl;

    //Generate the gradients for the line segments ...
    WaveCurve.init(&Wave);

    //Allocate the waveform structure ...
    if (!(waveForm= new WaveForm(&WaveCurve.b, &WaveCurve.c, &Wave))){
      cerr<<"VectorSynth::parseFig : Couldn't alloc waveForm"<<endl;
      return MALLOC_ERR;
    }
  } else {
    cerr<<"VectorSynth::parseFig : WavRange or Waveform line not found aborting"<<endl;
    exit(-1);
  }

  //Find and parse the volume ...
  if (Volume.getCount()){
    /*for (int i=0;i<Volume.getCount();i++)
      cout<<*Volume.grab(i+1)<<endl;
    */
    //cout<<"Reshaping Volume"<<endl;
    //cycle through the freqs shifting and re-scaling as we go
    lastX=-MAXDOUBLE;
    double minX=Volume.grab(1)->x, minY;
    double maxX=Volume.prev()->x, maxY;
    ERBRange.grab(1); ERBRange.prev();
    if (ERBRange.current()->y<ERBRange.next()->y) yMax=ERBRange.next()->y;
    else yMax=ERBRange.current()->y;
    yMin=ERBRange.next()->y;
    double height=yMin-yMax; height/=2.0;
    double width=maxX-minX;
    //cout<<"minX, maxX : "<<minX<<'\t'<<maxX<<endl;
    //cout<<"yMin, yMax : "<<yMin<<'\t'<<yMax<<endl;
    Volume.grab(1);
    for (int i=0;i<Volume.getCount();i++){
      if ((Volume.current()->x-=minX)<=lastX)//Fix X's which are b4 last X
	Volume.current()->x=lastX+2;
      lastX=Volume.current()->x;

      //Set volume zero to half way on octave scale
      Volume.current()->y=1.0-2.0*(Volume.current()->y-yMax)/(height);
      if (Volume.current()->y>1.0) Volume.current()->y=1.0;
      if (Volume.current()->y<0.0) Volume.current()->y=0.0;

      Volume.current()->x*=(xMax-xMin)/width;

      Volume.next();
    }

    /*for (int i=0;i<Volume.getCount();i++)
      cout<<*Volume.grab(i+1)<<endl;
    */
    //Generate the gradients for the line segments ...
    VolCurve.init(&Volume);
    //    for (int i=0;i<VolCurve.b.getCount();i++)
    //  cout<<VolCurve.b.grab(i+1)<<'\t'<<VolCurve.c.grab(i+1)<<endl;
  }
  cout<<"VectorSynth::parseFig : exit"<<endl;
  return 0;
}

int VectorSynth::
writeFile(const char *fName){
  ofstream output(fName);
  if (!output){
    cerr<<"VectorSynth::writeFile : Couldn't open file "<<fName<<endl;
    return FILEOPEN_ERR;
  }
  for (int i=0;i<outputAudio->audioSampleCount;i++)
    output<<outputAudio->audio[i]<<endl;
  output.close();
  return 0;
}

#ifdef HAVE_LIBSNDFILE
int VectorSynth::
writeWavFile(const char *fName){
  writeWav(fName, outputAudio->audioSampleCount, &outputAudio->audio[0], SAMPLE_RATE, 1/*channel cnt*/, 16/*bit cnt*/);
  return 0;
}
#endif

int VectorSynth::
processFile(const char *iFName){
  //cout<<"VectorSynth::processFile iFName : enter"<<endl;
  int res=0;
  if ((res=readFile(iFName))<0)
    return res;
  if ((res=generateSynth())<0)
    return res;

  struct stat figFileStatus;
  if (stat(iFName, &figFileStatus)<0){
    cerr<<"VectorSynth::processFile : fstat error returning 0"<<endl;
    return 0;
  }
  return figFileStatus.st_ctime;
}

/*int VectorSynth::
processFile(int iFID){
  //cout<<"VectorSynth::processFile iFID : enter"<<endl;
  int res=0;
  if ((res=readFile(iFID))<0)
    return res;
  if ((res=generateSynth())<0)
    return res;

  struct stat figFileStatus;
  if (fstat(iFID, &figFileStatus)<0){
    cerr<<"VectorSynth::processFile : fstat error returning 0"<<endl;
    return 0;
  }
  return figFileStatus.st_ctime;
  }*/

int VectorSynth::
generateSynth(void){
  cout<<"VectorSynth::generateSynth : enter "<<endl;
  while (changeOver) usleep(10000);
  /*cout<<"Freq count "<<Freq.getCount()<<endl;
  cout<<"ERB count "<<ERB.getCount()<<endl;
  cout<<"ERBRange count "<<ERBRange.getCount()<<endl;
  cout<<"FreqERB count "<<FreqERB.getCount()<<endl;
  cout<<"Volume count "<<Volume.getCount()<<endl;
  cout<<"WavRange count "<<WavRange.getCount()<<endl;
  cout<<"Wave count "<<Wave.getCount()<<endl;
  */
  double factor=(xMax-xMin)/(audio->audioSampleCount-1.0);
  //  cout<<audio->audioSampleCount<<endl;
  double where=0.0;
  double srcCnt=ERB.getCount();

  double a1=1000.0/C2;
  double a2=C3/log(10.0);
  // Find the line slopes -> b's
  double b[FreqERB.getCount()-1];
  double c[FreqERB.getCount()-1];
  double lastX=0.0, currentX=FreqERB.grab(1)->x;
  double lastY=0.0,currentY=FreqERB.current()->y;
  ERB.grab(1); ERB.prev();
  //  cout<<endl;
  for (int i=0;i<FreqERB.getCount()-1;i++){
    lastX=currentX;
    lastY=currentY;
    currentX=FreqERB.next()->x;
    currentY=FreqERB.current()->y;
    double diffX=time*(currentX-lastX)/(xMax-xMin);
    double diffY=currentY-lastY;
    //    cout<<diffX<<'\t'<<diffY<<endl;
    b[i]=diffY/diffX;
    c[i]=lastY;
    //    cout<<"Freq "<<Freq.current()<<" b="<<b[i]<<" c="<<c[i]<<endl;
  }
  //cout<<endl;

  ERB.grab(1); ERB.prev();
  Freq.grab(1); Freq.prev();
  double n=0.0, lastN=0.0;
  double amplitude=OUTPUTLEVEL/(double)srcCnt;
  double deg2rad=DEG2RAD, var[(int)srcCnt], lastVar[(int)srcCnt], lastVal[(int)srcCnt];
  double highFreq=-MAXDOUBLE, lowFreq=MAXDOUBLE, highOct=-MAXDOUBLE, lowOct=MAXDOUBLE, vol[(int)srcCnt];
  //Init the last values and find the lowest frequency
  for (int i=0;i<(int)srcCnt;i++){
    lastVal[i]=lastVar[i]=var[i]=0.0;
    if (Freq.next()<lowFreq) lowFreq=Freq.current();
    if (Freq.current()>highFreq) highFreq=Freq.current();
  }
  //cout<<"lowest Freq = "<<lowFreq<<endl;
  //cout<<"highest Freq = "<<highFreq<<endl;

    //Find the volume
    //    tempOct=grad*t+cStart;
    //vol=pow(2.0,-(log(Freq.current())/log(2.0)-log(lowFreq)/log(2.0)+tempOct-lowOct));
  for (int i=0;i<(int)srcCnt;i++){
    vol[i]=pow(2.0,-(log(Freq.next())/log(2.0)-log(lowFreq)/log(2.0)));
    //cout<<"Freq "<<Freq.current()<<" vol "<<vol[i]<<endl;
  }

  FreqERB.grab(1); FreqERB.prev();
  for (int i=0;i<FreqERB.getCount();i++){
    if (FreqERB.next()->y<lowOct) lowOct=FreqERB.current()->y;
    if (FreqERB.current()->y>highOct) highOct=FreqERB.current()->y;
  }
  //cout<<"lowest Octave = "<<lowOct<<endl;
  //cout<<"highest Octave = "<<highOct<<endl;

  //cout.precision(10);
  //  ofstream output("volume.txt");
  //ofstream output("temp.txt");
  //output.precision(20);
  Volume.grab(2);
  VolCurve.b.grab(1); VolCurve.c.grab(1);
  currentX=FreqERB.grab(1)->x;
  double t=0.0, volume, volXOffset=0.0;
  for (int i=0;i<FreqERB.getCount()-1;i++){
    currentX=FreqERB.next()->x;
    //    cout<<"currentX "<<currentX<<endl;
    double grad=b[i];
    double cStart=c[i];
    double rads, tempOct;
    //Small fix for clipping ...
    Freq.grab(1); Freq.prev();
    for (int j=0;j<srcCnt;j++){
      Freq.next();
      if (grad!=0.0){
	var[j]=lastVar[j]-deg2rad*(Freq.current()*(pow(2.0,cStart)/(grad*log(2.0))));
      } else {
	var[j]=lastVar[j];
      }
    }

    //cout<<"here"<<endl;
    for (n=fround(lastN);n<currentX/factor;n+=1.0){
      if (n*factor>Volume.current()->x) {
	Volume.next(); VolCurve.b.next(); VolCurve.c.next();
	volXOffset=n*factor;
      }
      //cout<<(double)n*factor-volXOffset<<'\t'<<VolCurve.b.current()<<'\t'<<VolCurve.c.current()<<endl;
      volume=VolCurve.b.current()*((double)n*factor-volXOffset)+VolCurve.c.current();
      //output<<volume<<endl;
      t=(n-lastN)/SAMPLE_RATE;
      audio->audio[(int)n]=0;
      for (int j=0;j<srcCnt;j++){
	ERB.next();
	Freq.next();
	// Find the radians
	if (grad!=0.0)
	  lastVar[j]=rads=(deg2rad*(Freq.current()*(pow(2.0,cStart+grad*t)/(grad*log(2.0))))+var[j]);
	else
	  lastVar[j]=rads=(deg2rad*(Freq.current()*(pow(2.0,cStart)*t))+var[j]);
	//	audio->audio[(int)n]+=(short int)rint(vol[j]*amplitude*(lastVal[j]=sin(rads)));
	audio->audio[(int)n]+=(short int)rint(volume*vol[j]*amplitude*(lastVal[j]=(*waveForm)[rads]));
      }
    }
    for (int j=0;j<(int)srcCnt;j++)
      var[j]=lastVar[j]; // Force continuity
    lastN=n;
  }
  //output.close();

  // Fade out the last 1 ms ...
  double outputLevel=OUTPUTLEVEL;
  double fadeTime=0.005; //seconds
  double fadeFact=1.0/(fround(fadeTime*SAMPLE_RATE)-1.0), fade=1.0+fadeFact;
  for (int i=audio->audioSampleCount-(int)fround(fadeTime*SAMPLE_RATE);i<audio->audioSampleCount;i++)
    audio->audio[i]=(short int)fround((double)audio->audio[i]*(fade-=fadeFact));

  //Indicate change over is ready ...
  changeOver=1;
  cout<<"VectorSynth::generateSynth : exit "<<endl;
  return 0;
}

void VectorSynth::
goChangeOver(void){
  cout<<"VectorSynth::goChangeOver enter"<<endl;
 // Copy the audio to the output buffer ...
  BassLine *temp=outputAudio;
  outputAudio=audio;
  audio=NULL;
  if (temp) delete temp;
  changeOver=0;

  cout<<"VectorSynth::goChangeOver exit"<<endl;
  //  writeFile(oFName);
}

#include "config.h"
#ifdef HAVE_EMSCRIPTEN
#include <emscripten/bind.h>
EMSCRIPTEN_BINDINGS(VectorSynth_ex) {
  emscripten::class_<VectorSynth>("VectorSynth")
  .constructor()
  .function("addFreq", &VectorSynth::addFreq)
  .function("setTime", &VectorSynth::setTime)
  .function("setRange", &VectorSynth::setRange)
  .function("addFreqRange", &VectorSynth::addFreqRange)
  .function("addFreqPoint", &VectorSynth::addFreqPoint)
  .function("addVolPoint", &VectorSynth::addVolPoint)
  .function("addWavRange", &VectorSynth::addWavRange)
  .function("addWavPoint", &VectorSynth::addWavPoint)
  .function("process", &VectorSynth::process)
  .function("generateSynth", &VectorSynth::generateSynth)
  .function("goChangeOver", &VectorSynth::goChangeOver)
  .function("getN", &VectorSynth::getN)
  .function("getSample", &VectorSynth::getSample)
  ;
}
#endif
