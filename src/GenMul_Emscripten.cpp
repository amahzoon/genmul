#include "GenMul_Emscripten.hpp"

string GenMul(int nIn1, int nIn2, int firstStage, int secondStage, int thirdStage)
{
    string result = moduleConnector(nIn1, nIn2, firstStage, secondStage, thirdStage);
    return result;
}

string GenMulNameMaker (int nIn1, int nIn2, int firstStage, int secondStage, int thirdStage) 
{
    string name = nameMaker (nIn1, nIn2, firstStage, secondStage, thirdStage);
    return name;
}

EMSCRIPTEN_BINDINGS(my_module1) {
    emscripten::function("GenMul", &GenMul);
}

EMSCRIPTEN_BINDINGS(my_module2) {
    emscripten::function("GenMulNameMaker", &GenMulNameMaker);
}
