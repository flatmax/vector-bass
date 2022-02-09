/* Copyright 1998, 1999, 2000, 2001 Matt Flax <flatmax@ieee.org>
   This file is now also part of MFFM VectorBass.

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

#include "Location.H"
#include <math.h>

// location based methods
loc::
loc(void){
  //  cout <<"loc: init " << this << endl;
  w=x=y=z=0.0;
}

loc::
loc(double _x, double _y, double _z){
  //cout <<"loc: init " << this << endl;
  w=0.0; x=_x; y=_y; z=_z;
}

loc::
loc(double _w, double _x, double _y, double _z){
  //cout <<"loc: init " << this << endl;
  w=_w; x=_x; y=_y; z=_z;
}

#define NORM(_x, _y, _z) sqrt(_x*_x + _y*_y + _z*_z)

loc loc::
findUnitVector(void){
  loc lNew;
  if (!(x==0.0 && y==0.0 && z==0.0)){
    double vecNorm =NORM(x, y, z);
    lNew=*this;
    lNew/=vecNorm;
  } // else return the zero vector
  //  cout <<lNew;
  return lNew;
}

loc loc::
findVectorFromAngle(double ang){
  double ang1=cos(ang);
  double ang2=-sin(ang);
  double ang3=sin(ang);
  double ang4=cos(ang);
  loc lNew;
  lNew.x=x*ang1+y*ang2;
  lNew.y=x*ang3+y*ang4;
  return lNew;
}

loc loc::
rotateX(double ang){
  double ang1=cos(ang);
  double ang2=-sin(ang);
  loc lNew;
  lNew.x=x;
  lNew.y=y*ang1+z*ang2;
  lNew.z=y*-ang2+z*ang1;
  return lNew;
}

loc loc::
rotateY(double ang){
  double ang1=cos(ang);
  double ang2=-sin(ang);
  loc lNew;
  lNew.x=x*ang1+z*-ang2;
  lNew.y=y;
  lNew.z=x*ang2+z*ang1;
  return lNew;
}

loc loc::
rotateZ(double ang){
  double ang1=cos(ang);
  double ang2=-sin(ang);
  loc lNew;
  lNew.x=x*ang1+y*ang2;
  lNew.y=x*-ang2+y*ang1;
  lNew.z=z;
  return lNew;
}

//#define DEG2RAD 2.0*M_PI/360.0 // For degrees to radians conversion
//#define DEG2RAD 0.01745329252

double loc::
findAngleBetween(loc that){
  double dotProd=findUnitVector().dot(that.findUnitVector());
  if (dotProd>1.0)
    dotProd=1.0;
  else if (dotProd<-1.0)
    dotProd=-1.0;
  return acos(dotProd);
}

double loc::
findAngleBetween(loc &unit1, loc &unit2){
  double dotProd=unit1.dot(unit2);
  if (dotProd>1.0)
    dotProd=1.0;
  else if (dotProd<-1.0)
    dotProd=-1.0;
  return acos(dotProd);
}

//operators
char loc::operator ==(loc& l){
  if (x==l.x && y==l.y && z==l.z)
    return 1;
  else
    return 0;
}

char loc::operator !=(loc& l){
  if (x!=l.x || y!=l.y || z!=l.z)
    return 1;
  else
    return 0;
}

loc loc::operator+(const loc& l){
  loc lNew;
  lNew.w=w+l.w; lNew.x=x+l.x;
  lNew.y=y+l.y; lNew.z=z+l.z;
  return lNew;
}
loc& loc::operator+=(const loc& l){
  w+=l.w; x+=l.x;
  y+=l.y; z+=l.z;
  return *this;
}
loc& loc::operator+=(const double l){
  w+=l; x+=l;
  y+=l; z+=l;
  return *this;
}
loc loc::operator-(void){
  loc lNew;
  lNew.w=-w; lNew.x=-x;
  lNew.y=-y; lNew.z=-z;
  return lNew;
}
loc loc::operator-(const loc& l){
  loc lNew;
  lNew.w=w-l.w; lNew.x=x-l.x;
  lNew.y=y-l.y; lNew.z=z-l.z;
  return lNew;
}
loc& loc::operator-=(const loc& l){
  w-=l.w; x-=l.x;
  y-=l.y; z-=l.z;
  return *this;
}

double loc::dot(const loc& l){
  return x*l.x+y*l.y+z*l.z;
}


loc loc::operator /(const double divisor){
  loc l;
  l.w=w/divisor; l.x=x/divisor;
  l.y=y/divisor; l.z=z/divisor;
  return l;
}
loc& loc::operator /=(const double divisor){
  w=w/divisor; x=x/divisor;
  y=y/divisor; z=z/divisor;
  return *this;
}
loc loc::operator *(const double divisor){
  loc l;
  l.w=w*divisor; l.x=x*divisor;
  l.y=y*divisor; l.z=z*divisor;
  return l;
}
loc& loc::operator *=(const double mult){
  w=w*mult; x=x*mult;
  y=y*mult; z=z*mult;
  return *this;
}
