# Copyright 1998, 1999, 2000, 2001 Matt Flax <flatmax@ieee.org>
#  This file is part of MFFM VectorBass.
#
#  MFFM VectorBass is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#   
#  MFFM VectorBass is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#   
#  You have received a copy of the GNU General Public License
#  along with MFFM VectorBass

CC=g++-3.0 -ggdb
CC=g++-3.0 -O2

LIBPATH=-L/home/flatmax/lib
LIBS=#-l3DPanner #-lSpline -lfitpack
COMPILE=$(CC) $(LIBPATH) $(LIBS)
.cc:$(COMPILE)  $<.cc -o $< 
.C.o:
	$(CC) -fPIC -c $<

SRC=vectorBass.C Location.C XFigOOutput.C
OBJ=$(SRC:.C=.o)

EXSRC=XFigParseTest.cc vectorBassTest.cc
EX=$(EXSRC:.cc=)

all: $(OBJ) \
	lib \
	ex

ex: $(OBJ)
	$(COMPILE) -lsndfile -L/home/flatmax/lib -lVB vectorBassTest.cc -o vectorBass
#	$(COMPILE) XFigParseTest.cc -o XFigParseTest
#	g++ -laudiofile audio2fig.C -o audio2fig
	$(CC) -lsndfile -L/home/flatmax/lib -lVB audio2fig.C -o audio2fig

lib:
	$(CC) -Wl,-shared,-soname,libVB.so.0.1 -o libVB.so.0.1 $(OBJ)

clean:
	rm -f *.tex *.mf *~ a.out $(EX) $(OBJ) lib* *.bak *.wav *.raw *.dat *.txt audio2fig vectorBass

vectorBass.o: vectorBass.H XFigParse.H curve.H waveForm.H
