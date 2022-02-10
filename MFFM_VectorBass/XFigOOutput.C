#include "XFigOOutput.H"

//Use this to write the header to the file ...
XFigOOutput::XFigOOutput(char *fileName) : ofstream(fileName){
#ifdef DEBUG_XFIGOOUTPUT
  cout<<"XFigOOutput::XFigOOutput("<<fileName<<")"<<endl;
#endif
  headerOut();
}
    
  //Use this to write the header to file and a line header too
XFigOOutput::XFigOOutput(char *fileName, int numberOfPoints, char* objectComment, XFigObject object) : ofstream(fileName){
#ifdef DEBUG_XFIGOOUTPUT
    cout<<"XFigOOutput::XFigOOutput("<<fileName<<")"<<endl;
#endif
    headerOut();

    //Output A range line in the case of a line Max to min ...
    *this<<"# "<<"wrline"<<'\n';
    *this<<object_code<<' '<<sub_type<<' '<<line_style<<' '<<thickness<<' '<<0<<' '<<fill_color<<' '<<depth<<' '<<pen_style<<' '<<area_fill<<' '<<style_val<<' '<<join_style<<' '<<cap_style<<' '<<radius<<' '<<forward_arrow<<' '<<backward_arrow<<' '<<(npoints=2)<<'\n';
    *this<<'\t';
    *this<<"1 "<<MAXWAVHEIGHT<<" 1 "<<-MAXWAVHEIGHT<<'\n';

    //Dump default object information
    *this<<"# "<<objectComment<<'\n';
    *this<<object_code<<' '<<sub_type<<' '<<line_style<<' '<<thickness<<' '<<pen_color<<' '<<fill_color<<' '<<depth<<' '<<pen_style<<' '<<area_fill<<' '<<style_val<<' '<<join_style<<' '<<cap_style<<' '<<radius<<' '<<forward_arrow<<' '<<backward_arrow<<' '<<(npoints=numberOfPoints)<<'\n';
    *this<<'\t';
  }

  void XFigOOutput::open(char *fileName) {
#ifdef DEBUG_XFIGOOUTPUT
      cout<<"XFigOOutput::open"<<endl;
#endif    
      ofstream::open(fileName);
      headerOut();
  }

  void XFigOOutput::headerOut(void){
#ifdef DEBUG_XFIGOOUTPUT
      cout<<"XFigOOutput::headerOut"<<endl;
#endif
    //Dump the desired header
    *this<<version<<'\n'<<orientation<<'\n'<<justification<<'\n'<<units<<'\n'<<papersize<<'\n'<<magnification<<'\n'<< multiple_page<<'\n'<<transparent_color<<'\n'<<comment<<'\n'<<resolution<<' '<<origin<<'\n';
  }

  void XFigOOutput::beginCompundedObjects(int ux, int uy, int lx, int ly){
    *this<<'6'<<' '<<ux<<' '<<uy<<' '<<lx<<' '<<ly<<'\n';
  }

  void XFigOOutput::endCompundedObjects(void){
    *this<<"-6"<<'\n';
  }

  //Use to start a new object in the same ope file ... defaults to no forward arrow and line object
  void XFigOOutput::newObject(int numberOfPoints, char* objectComment, int f_arrow, int p_colour, XFigObject object){
    if (p_colour>6)
      p_colour%=6;
    //Dump default object information
    *this<<"# "<<objectComment<<'\n';
    *this<<object_code<<' '<<sub_type<<' '<<line_style<<' '<<thickness<<' '<<p_colour<<' '<<fill_color<<' '<<depth<<' '<<pen_style<<' '<<area_fill<<' '<<style_val<<' '<<join_style<<' '<<cap_style<<' '<<radius<<' '<<f_arrow<<' '<<backward_arrow<<' '<<(npoints=numberOfPoints)<<'\n';
    *this<<'\t';
    if (f_arrow)//This is set to default values ...
      *this<<"0 0 1.00 60.00 120.00"<<"\n\t ";
  }

const char XFigOOutput::version[]="#FIG 3.2";
const char XFigOOutput::orientation[]="Landscape";//("Landscape" or "Portrait")
const char XFigOOutput::justification[]="Center";//("Center" or "Flush Left")
const char XFigOOutput::units[]="Metric";//("Metric" or "Inches")
const char XFigOOutput::papersize[]="A4";//("Letter", "Legal", "Ledger", "Tabloid","A", "B", "C", "D", "E", "A4",   "A3", "A2", "A1", "A0" and "B5")
const float XFigOOutput::magnification=100.0;//(export and print magnification, %)
const char XFigOOutput::multiple_page[]="Single";//("Single" or "Multiple" pages)
const int XFigOOutput::transparent_color=-2;//(color number for transparent color for GIF export. -2=None, -1=background, 0-31 for standard colors or 32- for user colors)
const char XFigOOutput::comment[]="# Author Matt Flax, MFFM: These can be changed in the file XFigParse.H - Check for mffm GPL & LGPL libs regularly @ sourceforge.net";
const int XFigOOutput::resolution=1200;// coord_system (Fig units/inch and coordinate system: 1: origin at lower left corner (NOT USED) 2: upper left)
const int XFigOOutput::origin=2;

  //XFig Object stuff
const int XFigOOutput::sub_type=1;//(1: polyline 2: box 3: polygon 4: arc-box) 5: imported-picture bounding-box)
const int XFigOOutput::line_style=0;//solid
const int XFigOOutput::thickness=1;//(in 1/80'ths of an inch)
const int XFigOOutput::pen_color=4;/*-1 = Default
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
  const int XFigOOutput::fill_color=0;//as for pen colour
  const int XFigOOutput::depth=500;//{0:999}
  const int XFigOOutput::pen_style=0;//solid
  const int XFigOOutput::area_fill=-1;//	(enumeration type, -1 = no fill)
  const float XFigOOutput::style_val=0.0;//(1/80 inch)
  const int XFigOOutput::join_style=0;//(enumeration type, 0=mitre)
  const int XFigOOutput::cap_style=0;//	(enumeration type, only used for POLYLINE, 0=Butt)
  const int XFigOOutput::radius=-1;//(1/80 inch, radius of arc-boxes) ?
  const int XFigOOutput::forward_arrow=0;//(0: off, 1: on)
  const int XFigOOutput::backward_arrow=0;//(0: off, 1: on)
