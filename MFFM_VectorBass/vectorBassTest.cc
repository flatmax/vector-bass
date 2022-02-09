#include "vectorBass.H"
/** Compile and run like so :
    g++ VectorBassTest.cc -o VectorBassTest -L/home/flatmax/lib -l3DPanner; ./VectorBassTest hello.fig
*/

//#define OUTPUTFNAME "output.dat"

int main(int argc, char **arg){
  if (argc != 3){
    cerr<<"Useage: "<<arg[0]<<" filename.fig otuput.wav"<<endl;
    exit(-1);
  }

  VectorBass vb;
  vb.processFile(arg[1]);
  while (!vb.changeOver);
  vb.goChangeOver();
  vb.writeWavFile(arg[2]);
  return 1;
}
