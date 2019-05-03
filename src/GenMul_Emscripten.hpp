#include "ModuleConnector.hpp"
#include <emscripten/bind.h>

string GenMul(int nIn1, int nIn2, int firstStage, int secondStage, int thirdStage); // create Multiplier
string GenMulNameMaker (int nIn1, int nIn2, int firstStage, int secondStage, int thirdStage);  //create Multiplier name
