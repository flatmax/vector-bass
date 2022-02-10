#ifndef XFIGOOUTPUT_H_
#define XFIGOOUTPUT_H_

#include <fstream>
using namespace std;

///Audio related defines below
#define MAXWAVHEIGHT 32768//2^(16)/2

class XFigOOutput : public ofstream {
  //XFig Header related stuff ... change at will - using a different constructor.
  const static char version[];
  const static char orientation[];//("Landscape" or "Portrait")
  const static char justification[];//("Center" or "Flush Left")
  const static char units[];//("Metric" or "Inches")
  const static char papersize[];//("Letter", "Legal", "Ledger", "Tabloid","A", "B", "C", "D", "E", "A4",   "A3", "A2", "A1", "A0" and "B5")
  const static float magnification;//(export and print magnification, %)
  const static char multiple_page[];//("Single" or "Multiple" pages)
  const static int transparent_color;//(color number for transparent color for GIF export. -2=None, -1=background, 0-31 for standard colors or 32- for user colors)
  const static char comment[];
  const static int resolution;// coord_system (Fig units/inch and coordinate system: 1: origin at lower left corner (NOT USED) 2: upper left)
  const static int origin;

  //XFig Object stuff
  char* object_comment;
  typedef enum {LINE=2} XFigObject;
  const static XFigObject object_code=LINE;//(always 2)
  const static int sub_type;//(1: polyline 2: box 3: polygon 4: arc-box) 5: imported-picture bounding-box)
  const static int line_style;//solid
  const static int thickness;//(in 1/80'ths of an inch)
  const static int pen_color;/*-1 = Default
	     0 = Black
	     1 = Blue
	     2 = Green
	     3 = Cyan
	     4 = Red
	     5 = Magenta
	     6 = Yellow
	     7 = White
	  8-11 = four shades of blue (dark to lighter)
	 12-14 = three shades of green (dark to lighter)
	 15-17 = three shades of cyan (dark to lighter)
	 18-20 = three shades of red (dark to lighter)
	 21-23 = three shades of magenta (dark to lighter)
	 24-26 = three shades of brown (dark to lighter)
	 27-30 = four shades of pink (dark to lighter)
	    31 = Gold
	    */
  const static int fill_color;//as for pen colour
  const static int depth;//{0:999}
  const static int pen_style;//solid
  const static int area_fill;//	(enumeration type, -1 = no fill)
  const static float style_val;//(1/80 inch)
  const static int join_style;//(enumeration type, 0=mitre)
  const static int cap_style;//	(enumeration type, only used for POLYLINE, 0=Butt)
  const static int radius;//(1/80 inch, radius of arc-boxes) ?
  const static int forward_arrow;//(0: off, 1: on)
  const static int backward_arrow;//(0: off, 1: on)
  int	npoints;

public:
  //Use this to write the header to the file ...
  XFigOOutput(char *fileName);
    
  //Use this to write the header to file and a line header too
  XFigOOutput(char *fileName, int numberOfPoints, char* objectComment, XFigObject object=LINE);

  void open(char *fileName);

  void headerOut(void);

  void beginCompundedObjects(int ux, int uy, int lx, int ly);

  void endCompundedObjects(void);

  //Use to start a new object in the same ope file ... defaults to no forward arrow and line object
  void newObject(int numberOfPoints, char* objectComment, int f_arrow=0, int p_colour=-1, XFigObject object=LINE);
};
#endif //XFIGOOUTPUT_H_
