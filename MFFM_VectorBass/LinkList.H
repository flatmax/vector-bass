/* Copyright 1998-2022 Flatmax Pty Ltd
   This file is now part of MFFM VectorSynth (previously VectorBass).

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

#ifndef LINKLIST_H
#define LINKLIST_H

#include <sys/types.h>
// #include <iomanip.h>
#include <iostream>
using namespace std;

//#define LINKLIST_DEBUG

template <class TYPE>
class Lug {

protected:

public:
//  unsigned char start; // 1 if 1st element 0 otherwise

  // Links to the next and prev elements in the list
  Lug<TYPE> * next, * prev;

  TYPE ptr; // The element to point to

  // This is for the first element - links to itself
  Lug(void){
    next = this;
    prev = this;
#ifdef LINKLIST_DEBUG
    cout <<"LinkList: Adding first "<<this<<" with previous "<< prev << " next " << next <<endl;
#endif
  }
  // Links to previous element and next element
  Lug(Lug<TYPE> * oldOne){
    next = (*oldOne).getNext();
    prev = oldOne;
#ifdef LINKLIST_DEBUG
    cout <<"LinkList: Adding "<<this<<" with previous "<< prev << " next " << next <<endl;
#endif
    (*oldOne).getNext()->prev = this;
    (*oldOne).next = this;
  }

  //  ~Lug(void){}
  
  // Return a pointer to the next element
  Lug *
  getNext(void){
    return next;
  }

  // Return a pointer to the previous element
  Lug *
  getPrev(void){
    return prev;
  }
};

template <class TYPE>
class LinkList {

  Lug<TYPE>* lug;
  Lug<TYPE>* startLug;
  long count;
public:
  // Direction ...
  typedef enum {FWD, REV} direction;
  direction dir;

  // Initialiser
  // This is for the first element - links to itself
  LinkList(void){
    lug = (Lug<TYPE>*)NULL;
    dir = FWD;
    count =0;
  }

  // Small cleanup ...
  ~LinkList(void){
    while (lug != (Lug<TYPE>*)NULL)
      remove();
  }


  // Adds as the current element ...
  void add(TYPE newElement){ // Add the ptr
    if (lug == (Lug<TYPE>*)NULL){ // create the first ...
      lug = new Lug<TYPE>;
      startLug = lug;
    } else // Add infront of the current lug
      lug = new Lug<TYPE>(lug);
    lug->ptr = newElement;
    count++;
  }

  TYPE change(TYPE changeElement){ // exchange ptr's
    if (lug != (Lug<TYPE>*)NULL){
      TYPE tempPtr = lug->ptr;
      lug->ptr = changeElement;
      return tempPtr;
    } else
      return (TYPE)NULL;
  }

  TYPE remove(void){ // Remove the current lug and return the ptr
    if (lug != (Lug<TYPE>*)NULL){
      if (lug == startLug)
	startLug = lug->getNext();
      Lug<TYPE>* tempLug = lug;
      TYPE tempPtr = lug->ptr;

      if (lug->getNext() == lug){ // Only one lug exists !
	lug = (Lug<TYPE>*)NULL;
      } else {
	lug->getPrev()->next = lug->getNext();
	lug->getNext()->prev = lug->getPrev();
	if (dir == FWD)
	  lug = lug->getNext();
	else
	  lug = lug->getPrev();
      }
      count--;
      delete tempLug;
      return tempPtr;
    } else {
      return (TYPE)0;//NULL;
    }
  }

  /* BAD IDEA ! - what if the lug->ptr is not a ptr !
  void delAll(void){
    while (getCount())
      del();
  }

  void del(void){ // Delete the current lug
    if (lug != (Lug<TYPE>*)NULL){
      if (lug == startLug)
	startLug = lug->getNext();
      Lug<TYPE>* tempLug = lug;
      TYPE tempPtr = lug->ptr;

      if (lug->getNext() == lug){ // Only one lug exists !
	lug = (Lug<TYPE>*)NULL;
      } else {
	lug->getPrev()->next = lug->getNext();
	lug->getNext()->prev = lug->getPrev();
	if (dir == FWD)
	  lug = lug->getNext();
	else
	  lug = lug->getPrev();
      }
      count--;
      delete tempLug;
      delete tempPtr;
      //      return tempPtr;
    }
  }
  */

  TYPE current(void){ // Return the current ptr
    if (lug != (Lug<TYPE>*)NULL)
      return lug->ptr;
    else
      return (TYPE)NULL;
  }

  TYPE next(void){ // return the next ptr
    if (lug != (Lug<TYPE>*)NULL){
      if (dir == FWD)
	lug = lug->getNext();
      else
	lug = lug->getPrev();    
      return lug->ptr;
    } else
      return (TYPE)0;//NULL;
  }

  TYPE prev(void){ // return the previous ptr
    if (lug != (Lug<TYPE>*)NULL){
      if (dir == FWD)
	lug = lug->getPrev();    
      else
	lug = lug->getNext();
      return lug->ptr;
    } else
      return (TYPE)0;//NULL;
  }

  TYPE grab(int i){ // return the i'th lug in the chain
    if (i<=0){
      cerr<<"LinkList: grab: index is less then or equal to zero"<<endl;
      return (TYPE)0;// NULL;
    } else if (i>count){
      cerr<<"LinkList: grab: index exceeds count"<<endl;
      return (TYPE)0;// NULL;
    } else {
      lug = startLug;

      while (--i)
	lug = lug->getNext();
      return lug->ptr;
    }
  }
  
  int getCount(void){return count;}

  friend ostream& operator <<(ostream& o, LinkList* l) {
    if (l->getCount()==0) return o;
    l->grab(1); l->prev();
    for (int j=0;j<l->getCount();j++)
      o<<l->next()<<' ';
    o<<endl;
    return o;
  }
  friend ostream& operator <<(ostream& o, LinkList& l) {
    if (l.getCount()==0) return o;
    l.grab(1); l.prev();
    for (int j=0;j<l.getCount();j++)
      o<<l.next()<<' ';
    o<<endl;
    return o;
  }
};
#endif // LINKLIST_H
