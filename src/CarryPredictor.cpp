#include "CarryPredictor.hpp"

CarryPredictor::CarryPredictor(vector<PartialProduct> insP, vector<PartialProduct> insG)
{
  this->inputsP = insP;
  this->inputsG = insG;

  int weight = insP.back().returnWeight() + 1; //weight of the output is equal to the weight of the last member of Propagate plus one
  PartialProduct out(weight);
  this->output.push_back(out);
}

PartialProduct CarryPredictor::returnOutput ()
{
  return this->output[0];
}

vector<int> CarryPredictor::collectIDs(vector<CarryPredictor*> vPredictor)
{
    vector<int> out;
    map<int, int> map;
    for (auto i: vPredictor)
    {
        for (auto j: i->inputsP)
            map[j.returnNo()] = 1;
        for (auto j: i->inputsG)
            map[j.returnNo()] = 1;
        for (auto j: i->output)
            map[j.returnNo()] = 1;
    }
    for (auto i: map)
    {
        out.push_back(i.first);
    }
    return out;
}

string CarryPredictor::returnVerilogCode(map<int, string> signalMap)
{
  vector<string> sVec; //a vector of strings to keep all products
  string sTemp;
  string out = "  assign " + signalMap[this->output[0].returnNo()] + " = ";

  vector<PartialProduct> PNew(this->inputsP.begin() + 1, this->inputsP.end());
  reverse(PNew.begin(), PNew.end());

  int neededIteration = this->inputsG.size();

  for (int i = 0; i < neededIteration; i++)
  {
    sTemp = "(";
    for (auto m : PNew)
    {
      sTemp = sTemp + signalMap[m.returnNo()] + " & ";
    }
    sTemp = sTemp + signalMap[this->inputsG[i].returnNo()] + ")";
    sVec.push_back(sTemp);
    sTemp = "";
    PNew.pop_back();
  }

  //create Sum of Products
  vector<string>::iterator it = sVec.begin();
  while (it!=sVec.end()-1)
  {
    out += *it + " | ";
    it++;
  }
  out+=sVec.back()+";";

  return out;
}