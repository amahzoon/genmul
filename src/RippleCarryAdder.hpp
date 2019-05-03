#include "VerilogGen.hpp"
#include <assert.h>
#include <iostream>

using namespace std;

int CreateRippleCarryAdder(int nIn1, int nIn2, string &file); // Get two integer numbers as input sizes and create the ripple carry adder