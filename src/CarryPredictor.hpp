#ifndef CP
#define CP

#include "partial.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class CarryPredictor
{ //components which are necessary in design
  private:
    vector<PartialProduct> inputsP;  //List of Propagates
    vector<PartialProduct> inputsG;  //List of Generates
    vector<PartialProduct> output; //predicted carry

  public:
    CarryPredictor(vector<PartialProduct> insP, vector<PartialProduct> insG);
    PartialProduct returnOutput (); //return the output of the predictor
    static vector<int> collectIDs(vector<CarryPredictor*> vComponent); //collect all used partial product IDs from predictor
    string returnVerilogCode (map<int, string> signalMap);
};

#endif
