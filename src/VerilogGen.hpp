// VerilogGen.cpp converts the generated circuit in the internal data structure to the excutable Verilog file

#ifndef VG
#define VG

#include <string>
#include <iostream>
#include <algorithm>
#include "component.hpp"
#include "CarryPredictor.hpp"

using namespace std;

void GenerateMainHeader(int nIn1, int nIn2, string &file);
void GenereateMainInOutSig(int nIn1, int nIn2, string &file);
void GenereateMainWires(map<int, int> PPGInfo, int nOut1PPA, int nOut2PPA, int nAddOut, string &file);

void GenerateHeader(int nIn1, int nIn2, string type, string &file);
void GenerateHeader(int nIn1, int nIn2, int nOut, string type, string &file);
void GenerateHeader(int nIn, string type, string &file);

void GenerateComponentHeader(int nIn1, int nIn2, int nIn1b, int singleBitNum, string type, string &file);
void GenerateComponentHeader(int nIn1, int nIn2, int nOut, string type, string &file);
void GenerateComponentHeader(int nIn, string type, string &file);

void GenereateInOutSig (int nIn1, int nIn2, int nOut, string &file);
void GenereateInOutSig(int nIn1, int nIn2, map<int, int> Outs, string &file);
void GenereateInOutSig(map<int, int> Ins, int nOut1, int nOut2, string &file);
map<int, string> generateWires (int nIn1, int nIn2, vector<int> signalIDs, vector<PartialProduct> outPartials, string &file);
map<int, string> generateWires(int nIn1, int nIn2, vector<int> signalIDs, map<int, vector<PartialProduct>> outPartials, string &file);
map<int, string> generateWires(map<int, int> Ins, vector<int> signalIDs, vector<PartialProduct> OutPar1, vector<PartialProduct> OutPar2, string &file);
void GenerateComponents (map<int, string>& signalMap, vector<Component*>& compList, string &file);
void GeneratePredictors (map<int, string> signalMap, vector<CarryPredictor*> predictorList, string &file);
void GenerateFullAdder (string &file);
void GenerateHalfAdder (string &file);
void GenerateFullAdderProp (string &file);
void GenerateConstantOne (string &file);
void GenerateCompressor (string &file);
void GenerateCounter (string &file);
string infoPrint (int firstInputLength, int secondInputLength, int firstStageID, int secondStageID, int thirdStageID);
#endif

