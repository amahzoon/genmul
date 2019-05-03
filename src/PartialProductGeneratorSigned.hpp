#include "VerilogGen.hpp"
#include <assert.h>
#include <iostream>

using namespace std;

map<int, int> PartialProductGeneratorSigned(int nIn1, int nIn2, string &file); // Get two integer numbers as input sizes and create the ripple carry adder