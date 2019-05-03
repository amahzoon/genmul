#include "VerilogGen.hpp"
#include <assert.h>
#include <iostream>
#include <cmath>

using namespace std;

int CarrySkipAdder(int nIn1, int nIn2, string &file); // Get two integer numbers as input sizes and create the carry skip adder