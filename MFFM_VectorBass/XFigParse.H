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

#ifndef XFIGPARSE_H_
#define XFIGPARSE_H_

//#define XFIGPARSE_DEBUG

#include <fstream>
#include <stdlib.h>
#include <values.h>
#include <math.h>

#include "Location.H"
#include "LinkList.H"

///Image related defines
#define HEIGHT 0.03 //In meters
#define PPC    400 // Points Per (PP) centimeter (approximatly 1200 PP inch)
//#defing PPM  4000// PP meter (approx)

///Audio Image Gain (AIGAIN - pronounced ey'gain): assume aim is at 1200 points
#define AIGAIN 1.8310546875e-02 //1200/65536

// XFigParse below
#define COMMENTID "#"
#define LINEIDaSTR "2"
#define LINEIDa 2
#define LINEIDb 1
#define FREQLINEID "fline"
#define RANGELINEID "rline"
#define VOLLINEID "vline"
#define WRANGELINEID "wrline"
#define WLINEID "wline"

#define DIVISOR 100.0

class XFigParse {

  /** Finds a comment, sets stream position to begining of location data.
      Returns number of location points
  */
  int findComment(ifstream *input, const char *comm){
    //    cout<<"find comment "<<comm<<endl;
    int ret=0;
    char tempW[1024];
    char tempC;
    //int tempI;
    double tempD;

    //find relevant comment
  research:
    while (input->get(tempC) && !ret){
      if (tempC == '#'){
	if (*input>>tempW && strcmp(comm, tempW)!=0){
	  goto research;
	} else{
	  // set stream position to the begining of the location data
	  // and get number of location points.
	  for (int i=0;i<16;i++)
	    *input >> tempD;
	  ret=(int)tempD;
	  //	  cout<<"Number of points for tag "<<comm<< " is "<<ret<<endl;
	}
      }
    }
    return ret;
  }

  double addLocs(ifstream *input, LinkList<loc *> *locs, int cnt){
    double tempD;
    loc *tempLoc;
    double x,y,z=0;
    double minX=MAXDOUBLE, minY=MAXDOUBLE;
#ifdef XFIGPARSE_DEBUG
    cout<<"XFigParse::addLocs: enter"<<endl;
#endif
    // Input the locs ....
    for (int i=0;i<cnt;i++){
      *input>>x>>y;
      if (!(tempLoc =new loc(x,y,z))){
	cerr<<"XFigParse:: loc malloc error "<<endl;
	exit(-1);
      }
      locs->add(tempLoc);
      if (x<minX) minX=x;
      if (y<minY) minY=y;
      //      cout<<*tempLoc<<endl;
    }

    // ensure X locs are > 0
    if (minX<=0)
      for (int i=0;i<locs->getCount();i++)
	locs->next()->x-=(minX-1);
    // ensure Y locs are > 0
    if (minY<=0)
      for (int i=0;i<locs->getCount();i++)
	locs->next()->y-=(minY-1);

    input->seekg(-1, ios::cur);
#ifdef XFIGPARSE_DEBUG
    cout<<"XFigParse::addLocs: exit"<<endl;
#endif
    return tempD;
  }

  //Removes and deletes all locs and loc linked list
  void remLocs(LinkList<loc *> *locs){
    loc *tempLoc;
    while (locs->getCount()){
      tempLoc=locs->remove();
      delete tempLoc;
    }
    delete locs;
  }

  void checkReverseX(LinkList<loc *> *line){
      // Check wether the freqerb line needs reversing ...
      if (line->grab(1)->x>line->grab(line->getCount())->x){
	LinkList<loc *> tempERB;
	line->grab(1); line->prev();
	line->dir=LinkList<loc *>::REV;
	while (line->getCount()) // Remove reverse order ...
	  tempERB.add(line->remove());
	line->dir=LinkList<loc *>::FWD;
	
	tempERB.grab(1);
	while (tempERB.getCount()) // Replace forward order
	  line->add(tempERB.remove());
      }      
  }

  void checkReverseY(LinkList<loc *> *line){
      // Check wether the freqerb line needs reversing ...
      if (line->grab(1)->y>line->grab(line->getCount())->y){
	LinkList<loc *> tempERB;
	line->grab(1); line->prev();
	line->dir=LinkList<loc *>::REV;
	while (line->getCount()) // Remove reverse order ...
	  tempERB.add(line->remove());
	line->dir=LinkList<loc *>::FWD;
	
	tempERB.grab(1);
	while (tempERB.getCount()) // Replace forward order
	  line->add(tempERB.remove());
      }      
  }

  //Finds the closest line to the range line, with ID 'lineID' and returns 'retLine'
  void findClosestLine(ifstream *input, const char *lineID, LinkList<loc *> *rangeLine, LinkList<loc *> *retLine){
#ifdef XFIGPARSE_DEBUG
    cout<<"findClosestLine enter : "<<lineID<<endl;
#endif
    int cnt=0;
    // Find and fill all possible retLine lines ...
    LinkList<LinkList<loc *> *> lineHolder;
    int currentPos;
    while (cnt=findComment(input, lineID)){
      currentPos=input->tellg();
      LinkList<loc *> *tempLocs=new LinkList<loc *>;
      addLocs(input, tempLocs, cnt);
      checkReverseX(tempLocs);
      //      for (int i=1;i<=tempLocs->getCount();i++)
      //cout<<*tempLocs->grab(i)<<endl;
      lineHolder.add(tempLocs);
      //Reset the old position and keep searching ...
      input->clear();
      input->seekg(currentPos, ios::beg);
    }
    //Find the closest line to the highest range marker ....
    double highX=rangeLine->grab(1)->x, highY=rangeLine->grab(1)->x, minDist=MAXDOUBLE, val;
    int which, whichX, whichY;
    if (lineHolder.getCount()){
      lineHolder.grab(1);
      for (int i=0;i<lineHolder.getCount();i++){
	if ((val=sqrt(pow(2.0,lineHolder.current()->grab(1)->x/DIVISOR-highX/DIVISOR)+pow(2.0,lineHolder.current()->grab(1)->y/DIVISOR-highY/DIVISOR)))<minDist){
	  minDist=val;
	  which=i+1;
	}
	lineHolder.next();
      }
      //Set the wanted line ...
      lineHolder.grab(which); lineHolder.current()->grab(1);
      while (lineHolder.current()->getCount())
	retLine->add(lineHolder.current()->remove());
      
      //Delete the unwanted lines
      while (lineHolder.getCount())
	remLocs(lineHolder.remove());
    }
#ifdef XFIGPARSE_DEBUG
    cout<<"findClosestLine exit"<<endl;
#endif
  }

  int findRangeLine(ifstream *input, const char *lineID, LinkList<loc *> *retLine){
#ifdef XFIGPARSE_DEBUG
    cout<<"findRangeLine enter : "<<lineID<<endl;
#endif
    int cnt=0;
    if (cnt=findComment(input, lineID)){
      addLocs(input, retLine, cnt);
      if (retLine->getCount()==0)
	goto error;
      checkReverseY(retLine);
      //Remove all but the inportant exteremes of the line ...
      loc *tempLoc;

      retLine->grab(1); retLine->next();
      while (retLine->getCount()>2){
	tempLoc=retLine->remove();
	delete tempLoc;
      }
    } else
      goto error;
#ifdef XFIGPARSE_DEBUG
    cout<<"findRangeLine exit"<<endl;
#endif
    return 0;
  error:
#ifdef XFIGPARSE_DEBUG
    cout<<"findRangeLine exit : error"<<endl;
#endif
    return -1;
  }

public:
  //XFigParse(void){cout<<"start"<<endl;}

  void getVariables(LinkList<double> *Freq,
		    double *time,
		    double *range,
		    LinkList<loc *> *ERBRange,
		    LinkList<loc *> *FreqERB, 
		    LinkList<loc *> *Volume, 
		    LinkList<loc *> *WavRange, 
		    LinkList<loc *> *Wave, ifstream *input){
    //#ifdef XFIGPARSE_DEBUG
    cout<<"XFigParse::getVariable : enter"<<endl;
    //#endif
    double tempD;
    char tempC;
    char tempW[1024];

    input->clear();
    input->seekg(0);
    // First find the freqs ....
    while (*input>>tempW && strcmp("Freq", tempW)!=0);
    while (*input>>tempD) Freq->add(tempD);
    cout <<Freq->getCount()<<" Frequencies found : ";
    for (int i=0;i<Freq->getCount();i++)
      cout<<Freq->next()<<'\t';
    cout<<endl;

    input->clear();
    input->seekg(0);
    // Find the time ...
    while (*input>>tempW && strcmp("Time", tempW)!=0);
    *input>>tempD; *time=tempD;
    cout <<"Length (time) set to : "<<*time<<endl;
    
    input->clear();
    input->seekg(0);
    // Find the Range ...
    while (*input>>tempW && strcmp("Range", tempW)!=0);
    *input>>tempD; *range=tempD;
    cout <<"Range set to +-"<<*range<<endl;

    input->clear();
    input->seekg(0);
    // Find and fill the Range line ...
    if (findRangeLine(input, RANGELINEID, ERBRange)<0)
      cout<<"XFigParse:: no range line found for id : "<<RANGELINEID<<endl;
    //for (int j=1;j<=ERBRange->getCount();j++)
    //cout<<*ERBRange->grab(j)<<endl;

    input->clear();
    input->seekg(0);
    //Find the frequency curve ...
    findClosestLine(input, FREQLINEID, ERBRange, FreqERB);

    //for (int i=1;i<=FreqERB->getCount();i++)
    //cout<<*FreqERB->grab(i)<<endl;

    input->clear();
    input->seekg(0);
    //Find the frequency curve ...
    findClosestLine(input, VOLLINEID, ERBRange, Volume);
    //for (int i=1;i<=Volume->getCount();i++)
    //cout<<*Volume->grab(i)<<endl;

    input->clear();
    input->seekg(0);
    // Find and fill the Waveform Range line ...
    if (findRangeLine(input, WRANGELINEID, WavRange)<0)
      cout<<"XFigParse:: no range line found for id : "<<WRANGELINEID<<endl;

    //for (int j=1;j<=WavRange->getCount();j++)
    //  cout<<*WavRange->grab(j)<<endl;

    input->clear();
    input->seekg(0);
    // Find and fill the Waveform line ...
    findClosestLine(input, WLINEID, WavRange, Wave);
    //for (int j=1;j<=Wave->getCount();j++)
    //cout<<*Wave->grab(j)<<endl;

    input->clear();
    input->seekg(0);
    cout<<"XFigParse::getVariable : exit"<<endl;    
  }
};
#endif//XFIGPARSE_H_
