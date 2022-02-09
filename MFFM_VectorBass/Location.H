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

#ifndef LOCATION_H_
#define LOCATION_H_

#include <iomanip.h>

/// An omni, x, y, and z location vector
class loc {
public:
  /**
     omni value and x, y, z location
  */
  double w, x, y, z;

  /// Initialise to origin, omni = 0
  loc(void);
  /// Init to _x, _y, _z and omni = 0
  loc(double _x, double _y, double _z);
  /// Init to _w, _x, _y, _z
  loc(double _w, double _x, double _y, double _z);

  /// Returns the unit vector
  loc findUnitVector(void);
  // Returns the vector given the angle (only works in the x/y plane)
  loc findVectorFromAngle(double ang);
  /// Returns the vector rotated about the x-axis (ang is in radians)
  loc rotateX(double ang);
  /// Returns the vector rotated about the y-axis (ang is in radians)
  loc rotateY(double ang);
  /// Returns the vector rotated about the z-axis (ang is in radians)
  loc rotateZ(double ang);
  /// Returns the angle between this and that (in radians)
  double findAngleBetween(loc that);
  /// Returns the angle between the two unit vectors (more efficient)
  double findAngleBetween(loc &unit1, loc &unit2);

  // operators
  /// operator <<: the output operator
  friend ostream& operator <<(ostream& o, const loc& l) {
    o << "loc: "<<l.w <<' '<<l.x<<' '<<l.y<<' '<<l.z;
    return o;
  }

  /** 
      Boolean operators, don't operate on omni
  */
  /// operator ==: equals comparison
  char operator ==(loc& l);
  /// operator !=: not equal to comparison
  char operator !=(loc& l);

  /** 
      loc based operators, operate on omni as well
      
      operator +: addition operator
  */
  loc operator +(const loc& l);
  /// operator +=: self addition operator
  loc& operator +=(const loc& l);
  /// operator +=: scalar self addition operator
  loc& operator +=(const double l);
  /// operator -: negation operator
  loc operator -(void);
  /// operator -: subtraction operator
  loc operator -(const loc& l);
  /// operator -=: self subtraction operator
  loc& operator -=(const loc& l);

  /// dot: dot product
  double dot(const loc& l);

  /**
     Scalar based operators, operate on omni as well
     
     operator /: division operator
  */
  loc operator /(const double divisor);
  /// operator /=: self division operator
  loc& operator /=(const double divisor);
  /// operator *: multiplication operator
  loc operator *(const double divisor);
  /// operator *=: self multiplication operator
  loc& operator *=(const double mult);
};
#endif //LOCATION_H_
