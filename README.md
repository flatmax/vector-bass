#Vector Graphic Audio Synth.

Vector synth is an example of a few things :

* A melody line generation tool which lets you plot the pitch, volume and
   'generic waveform sound'. This means you can essentially hear a piece of
   music, grab and plot the synth. waveform and generate your own melody lines
   using this synth. Whether the synth be a persons voice, waveform or whatever !
   This is accomplished by merging a FM (frequency modulator) and a Vector
   Graphics application (XFig).
* Rapid prototyping using a vector graphics application as the front end
   and a custom engine as the back end.


The original Vector bass from 1998 : https://sourceforge.net/projects/mffmvectorbass/

The original website : http://mffmvectorbass.sourceforge.net/

Latest repo : https://github.com/flatmax/vector-synth

# C++ or WASM

This is a project foundation for C++ and WASM.
The output from the C++ are ES6 WASM modules for nodejs and the browser (loaded through an example webcomonent).
You can test the output for this pacakge using npm : https://www.npmjs.com/package/vector-synth

# Running the nodejs version

A test executable : VectorSynthTestNodeTest.js

# Running the webapp

Open a new terminal, then :
```
cd webcomponent
```
### installing
```
npm i
```
### running
```
polymer serve # or user a different serving mechanism if you don't have polymer
```
# Building
## Initial Setup
```
./autogen.sh
```
### WASM
If you are building Web Assembly :
```
source /usr/local/emsdk/emsdk_env.sh
```
## configuring
### native C++
```
./configure
```
### WASM
```
emconfigure ./configure
```
## building
### native C++
```
make
```
### WASM
```
emmake make
```
## installing
```
emmake make install
```
