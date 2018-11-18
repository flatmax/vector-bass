# WASM

This is a project foundation for C++ and WASM.

# Building
## Initial Setup

./tools/autotools.sh

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

./test/RunTest.js

# Running the webapp

Open a new terminal, then :

cd webApp

### installing

polymer install

### running

polymer serve
