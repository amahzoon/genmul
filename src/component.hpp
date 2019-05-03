#ifndef COM
#define COM


#include "partial.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

class Component
{ //components which are necessary in design
  protected:
    vector<PartialProduct> inputs;  // inputs of the component
    vector<PartialProduct> outputs; // outputs of the component

  public:
    Component(vector<PartialProduct> ins);
    vector<PartialProduct> returnOutputs (); //return the outputs of the component
    vector<PartialProduct> returnInputs (); //return the inputs of the component
    static vector<int> collectIDs(vector<Component*> vComponent); //collect all unig partial product IDs from components
    virtual void SetOutputs(); //calculate the output and set it
    virtual string returnVerilogCode (map<int, string>& signalMap, int ID) =0;
};

// class HalfAdder is a child for Component
class HalfAdder : public Component
{
  public:
    HalfAdder(vector<PartialProduct> ins): Component(ins) {}
    string returnVerilogCode (map<int, string>& signalMap, int ID);

};

// class FullAdder is a child for Component
class FullAdder : public Component
{
  public:
    FullAdder(vector<PartialProduct> ins): Component(ins) {}
    string returnVerilogCode (map<int, string>& signalMap, int ID);
};

// class FullAdderProp is a child for Component (same as FUllAdder but generates propagate signals)
class FullAdderProp : public Component
{
  public:
    FullAdderProp(vector<PartialProduct> ins): Component(ins) {}
    void SetOutputs(); //calculate the output and set it
    string returnVerilogCode (map<int, string>& signalMap, int ID);
};

// class Compresseor is a child for Component
class Compressor : public Component
{
  public:
    Compressor(vector<PartialProduct> ins): Component(ins) {}
    void SetOutputs(); //calculate the output and set it
    string returnVerilogCode (map<int, string>& signalMap, int ID);
};

// class Compresseor is a child for Component
class Counter : public Component
{
  public:
    Counter(vector<PartialProduct> ins): Component(ins) {}
    void SetOutputs(); //calculate the output and set it
    string returnVerilogCode (map<int, string>& signalMap, int ID);
};

// class XOR is a child for Component
class XOR : public Component
{
  public:
    XOR(vector<PartialProduct> ins): Component(ins) {}
    void SetOutputs(); //calculate the output and set it
    string returnVerilogCode (map<int, string>& signalMap, int ID);
};

// class AND is a child for Component
class AND : public Component
{
  public:
    AND(vector<PartialProduct> ins): Component(ins) {}
    void SetOutputs(); //calculate the output and set it
    string returnVerilogCode (map<int, string>& signalMap, int ID);
};

// class NAND is a child for Component
class NAND : public Component
{
  public:
    NAND(vector<PartialProduct> ins): Component(ins) {}
    void SetOutputs(); //calculate the output and set it
    string returnVerilogCode (map<int, string>& signalMap, int ID);
};

// class BUF is a child for Component
class BUF : public Component
{
  public:
    BUF(vector<PartialProduct> ins): Component(ins) {}
    void SetOutputs(); //calculate the output and set it
    string returnVerilogCode (map<int, string>& signalMap, int ID);
};

//class CarryOperator is a child of Component
class CarryOperator: public Component //operation: P = P'&P"      G = G"|(G'&P")
{
  public:
    CarryOperator(vector<PartialProduct> ins): Component(ins){} //the inputs oredr: 0:P'     1: G'     2:P"       3:G"
    void SetOutputs(); //calculate the output and set it
    string returnVerilogCode (map<int, string>& signalMap, int ID);
};

// class Skip is a child for Component
class Skip : public Component
{
  public:
    Skip(vector<PartialProduct> ins): Component(ins) {} //the first input is the OR input anf the rest are AND inputs
    void SetOutputs(); //calculate the output and set it
    string returnVerilogCode (map<int, string>& signalMap, int ID);

};


class ConstantOne : public Component
{
  public:
    ConstantOne(vector<PartialProduct> ins): Component(ins) {}
    void SetOutputs(); //calculate the output and set it
    string returnVerilogCode (map<int, string>& signalMap, int ID);
};

#endif

