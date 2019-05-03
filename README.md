[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

# GenMul (version 1)

GenMul is a multiplier generator which outputs multiplier circuits in Verilog. The input size of a multiplier and each multiplier stage can be configured with GenMul. For more information visit [sca-verification website](http://www.sca-verification.org).

## Clone

Clone GenMul using:

```bash
git clone https://github.com/amahzoon/genmul.git
```

## Installation (shell interface)

To build GenMul binary:

```bash
mkdir build
cd build
cmake ..
make install -j2
```

After installation, GenMul can be run from `bin\genmul`. 

## Installation (Emscripten)

[Emscripten](https://emscripten.org/) toolchain can be used to compile JavaScript from our C++ implementation of GenMul.

```bash
mkdir build
cd build
emconfigure cmake ..
emmake make install -j2
```

The compiled files are accessible through `bin\genmul.js` and `bin\genmul.wasm`.

Currecntly, we have used the compiled javaScript files in the [web-based version of Genmul](http://www.sca-verification.org/genmul).

## Getting Started

```bash
./genmul
```

After running, GenMul ask you to Choose the architectures for the Partial Product Generator (PPG), Partial Product Accumulator (PPA), and Final Stage Adder (FSA), respectively. Then, the bit sizes of the first and second inputs have to be set. Finally, the Verilog file of the multiplier is generated.

