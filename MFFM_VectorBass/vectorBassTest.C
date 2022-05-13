#include "vectorBass.H"

int main(int argc, char **arg){
  if (argc != 2){
    cerr<<"Useage: "<<arg[0]<<" output.wav"<<endl;
    exit(-1);
  }

  VectorBass vb;

  // the following data is taken from newhello.fig

  vb.addFreq(330.); // can add more then  one
  vb.setTime(5.); // duration in seconds
  vb.setRange(24.); // Number of piano semitones

  vb.addFreqRange(990., 900., 225., 3600.); // y axis over which the setRange semitones exist

  // add some pitch variation curves
  vb.addFreqPoint(1800., 4140.);
  vb.addFreqPoint(1369., 2956.);
  vb.addFreqPoint(2045., 2710.);
  vb.addFreqPoint(2477., 3894.);
  vb.addFreqPoint(2976., 2371.);

  // volume changes
  vb.addVolPoint(1800., 901.);
  vb.addVolPoint(2250., 900.);
  vb.addVolPoint(2700., 900.);
  vb.addVolPoint(3150., 900.);
  vb.addVolPoint(3600., 900.);
  vb.addVolPoint(4050., 900.);
  vb.addVolPoint(4500., 900.);
  vb.addVolPoint(4950., 900.);
  vb.addVolPoint(5400., 900.);
  vb.addVolPoint(6255., 899.);

  // waveform y range for max amplitude and min amplitude
  vb.addWavRange(225., 6750., 225., 8100.);

  // define the waveform to oscillate
  vb.addWavPoint(360., 7425.);
  vb.addWavPoint(630., 6750.);
  vb.addWavPoint(1035., 8100.);
  vb.addWavPoint(1260., 7425.);

  int ret;
  if ((ret=vb.process())!=0)
    return ret;
  if ((ret=vb.generateBass())<0)
    return ret;

  while (!vb.changeOver);
  vb.goChangeOver();
  vb.writeWavFile(arg[1]);
  return 1;
}
