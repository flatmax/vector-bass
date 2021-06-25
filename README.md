# WASM

This is a project foundation for C++ and WASM.
The output from the C++ are ES6 WASM modules for nodejs and the browser (loaded through an example webcomonent).
You can test the output for this pacakge using npm : https://www.npmjs.com/package/libwasm
You can test the output for this packaing using nodejs in the nodejs directory.

# Building
## Initial Setup

./tools/autotools.sh

### WASM

source /usr/local/emsdk/emsdk_env.sh

## configuring
### native C++
./configure

### WASM
emconfigure ./configure

## building
### native C++
make
### WASM
emmake make

## installing

Really ?

# Running the nodejs version

./nodejs/WASMTestNode.js

# Running the webapp

Open a new terminal, then :

cd webcomponent

### installing

npm i

### running

polymer serve # or user a different serving mechanism if you don't have polymer
