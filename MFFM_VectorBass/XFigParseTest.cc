#include <LinkList.H>

#include "XFigParse.H"
#include "../2daudio/Location.H"

/** Compile and run like so :
    g++ XFigParseTest.cc -o XFigParseTest -L/home/flatmax/lib -l3DPanner; ./XFigParseTest hello.fig
*/

LinkList<double> Freq;
double time;
double range;
LinkList<loc *> ERBRange;
LinkList<loc *> FreqERB;
LinkList<loc *> Volume;
LinkList<loc *> WavRange;
LinkList<loc *> Wave;

void main(int argc, char **arg){
  //for (int i=0;i<argc;i++)
  //  cout<<arg[i]<<endl;

  if (argc != 2){
    cerr<<"Useage: "<<arg[0]<<" filtename.fig"<<endl;
    exit(-1);
  }

  ifstream input(arg[1]);
  XFigParse parser;
  parser.getVariables(&Freq, &time, &range, &ERBRange, &FreqERB, &Volume, &WavRange, &Wave, &input);
}
