#include "component.hpp"
#define UNUSED(x) (void)(x)

Component::Component(vector<PartialProduct> ins)
{
    this->inputs = ins;
}

vector<PartialProduct> Component::returnOutputs()
{
    return this->outputs;
}

vector<PartialProduct> Component::returnInputs()
{
    return this->inputs;
}

void Component::SetOutputs() //default for HA and FA
{
    int partialsWeight = this->inputs[0].returnWeight(); //the weights of all partial products are same, so using one of them is sufficient
    PartialProduct s(partialsWeight);                    //the sum output of the adder
    PartialProduct c(partialsWeight + 1);                //the carry output of the adder
    this->outputs.push_back(s);                          //the first output is always sum
    this->outputs.push_back(c);                          //the second output is always carry
}

void Compressor::SetOutputs() 
{
    int partialsWeight = this->inputs[0].returnWeight(); //the weights of all partial products are same, so using one of them is sufficient
    PartialProduct s(partialsWeight);                    //the sum output of the adder
    PartialProduct c(partialsWeight + 1);                //the first carry output of the adder
    PartialProduct co(partialsWeight + 1);               //the second carry output of the adder
    this->outputs.push_back(s);                          //the first output is always sum
    this->outputs.push_back(c);                          //the second output is always the first carry
    this->outputs.push_back(co);                          //the third output is always the second carry
}

void Counter::SetOutputs() 
{
    int partialsWeight = this->inputs[0].returnWeight(); //the weights of all partial products are same, so using one of them is sufficient
    PartialProduct s(partialsWeight);                    //the sum output of the adder
    PartialProduct c(partialsWeight + 1);                //the first carry output of the adder
    PartialProduct co(partialsWeight + 2);               //the second carry output of the adder
    this->outputs.push_back(s);                          //the first output is always sum
    this->outputs.push_back(c);                          //the second output is always the first carry
    this->outputs.push_back(co);                          //the third output is always the second carry
}

void FullAdderProp::SetOutputs()
{
    int partialsWeight = this->inputs[0].returnWeight(); //the weights of all partial products are same, so using one of them is sufficient
    PartialProduct s(partialsWeight);                    //the sum output of the adder
    PartialProduct c(partialsWeight + 1);                //the carry output of the adder
    PartialProduct p(partialsWeight);                    //the propagte output of the adder
    this->outputs.push_back(s);                          //the first output is always sum
    this->outputs.push_back(c);                          //the second output is always carry
    this->outputs.push_back(p);                          //the second output is always carry
}

void XOR::SetOutputs()
{
    int partialsWeight = this->inputs[0].returnWeight(); //the weights of all partial products are same, so using one of them is sufficient
    PartialProduct s(partialsWeight);                    //there is just one output with the weight same as inputs
    this->outputs.push_back(s);                          
}

void AND::SetOutputs()
{
    int partialsWeight = this->inputs[0].returnWeight() + this->inputs[1].returnWeight(); //the weight of final product is the sumation od input partial products
    PartialProduct a(partialsWeight);                    //there is just one output 
    this->outputs.push_back(a);                         
}


void NAND::SetOutputs()
{
    int partialsWeight = this->inputs[0].returnWeight() + this->inputs[1].returnWeight(); //the weight of final product is the sumation od input partial products
    PartialProduct a(partialsWeight);                    //there is just one output 
    this->outputs.push_back(a);                         
}

void BUF::SetOutputs()
{
    int partialsWeight = this->inputs[0].returnWeight(); //the weight of final product is same as the input partial products
    PartialProduct a(partialsWeight);                    //there is just one output 
    this->outputs.push_back(a);                         
}

void Skip::SetOutputs()
{
    int partialsWeight = this->inputs[0].returnWeight(); //the weight of final product is same as the first input partial product
    PartialProduct a(partialsWeight);                    //there is just one output 
    this->outputs.push_back(a);                         
}

void ConstantOne::SetOutputs()
{
    int partialsWeight = this->inputs[0].returnWeight(); //the weight of final product is same as the input partial products
    PartialProduct a(partialsWeight);                    //there is just one output 
    this->outputs.push_back(a);                         
}

vector<int> Component::collectIDs(vector<Component *> vComponent)
{
    vector<int> out;
    map<int, int> map;
    for (auto i : vComponent)
    {
        for (auto j : i->inputs)
            map[j.returnNo()] = 1;
        for (auto j : i->outputs)
            map[j.returnNo()] = 1;
    }
    for (auto i : map)
    {
        out.push_back(i.first);
    }
    return out;
}

void CarryOperator::SetOutputs()
{
    int partialsWeight;
    if (this->inputs.size() == 2) //if there is two inputs (P' and P"), it means that G is equal to zero
    {
        partialsWeight = this->inputs[1].returnWeight();
        PartialProduct P(partialsWeight);
        this->outputs.push_back(P);
    }
    else
    {
        partialsWeight = this->inputs[2].returnWeight();
        PartialProduct P(partialsWeight);
        this->outputs.push_back(P);
        PartialProduct G(partialsWeight + 1);
        this->outputs.push_back(G);
    }
}

string HalfAdder::returnVerilogCode(map<int, string>& signalMap, int ID)
{
    string out = "  HalfAdder U" + to_string(ID) + " (" + signalMap[this->inputs[0].returnNo()] + ", " + signalMap[this->inputs[1].returnNo()] + ", " + signalMap[this->outputs[0].returnNo()] + ", " + signalMap[this->outputs[1].returnNo()] + ");";
    return out;
}

string FullAdder::returnVerilogCode(map<int, string>& signalMap, int ID)
{
    string out = "  FullAdder U" + to_string(ID) + " (" + signalMap[this->inputs[0].returnNo()] + ", " + signalMap[this->inputs[1].returnNo()] + ", " + signalMap[this->inputs[2].returnNo()] + ", " + signalMap[this->outputs[0].returnNo()] + ", " + signalMap[this->outputs[1].returnNo()] + ");";
    return out;
}

string FullAdderProp::returnVerilogCode(map<int, string>& signalMap, int ID)
{
    string out = "  FullAdderProp U" + to_string(ID) + " (" + signalMap[this->inputs[0].returnNo()] + ", " + signalMap[this->inputs[1].returnNo()] + ", " + signalMap[this->inputs[2].returnNo()] + ", " + signalMap[this->outputs[0].returnNo()] + ", " + signalMap[this->outputs[1].returnNo()] + ", " + signalMap[this->outputs[2].returnNo()] + ");";
    return out;
}

string Compressor::returnVerilogCode(map<int, string>& signalMap, int ID)
{
    string out = "";
    if (this->inputs.size()==5)
        out = "  Compressor U" + to_string(ID) + " (" + signalMap[this->inputs[0].returnNo()] + ", " + signalMap[this->inputs[1].returnNo()] + ", " + signalMap[this->inputs[2].returnNo()]+ ", " + signalMap[this->inputs[3].returnNo()]+ ", " + signalMap[this->inputs[4].returnNo()] + ", " + signalMap[this->outputs[0].returnNo()] + ", " + signalMap[this->outputs[1].returnNo()]+ ", " + signalMap[this->outputs[2].returnNo()] + ");";
    else
        out = "  Compressor U" + to_string(ID) + " (" + signalMap[this->inputs[0].returnNo()] + ", " + signalMap[this->inputs[1].returnNo()] + ", " + signalMap[this->inputs[2].returnNo()]+ ", " + signalMap[this->inputs[3].returnNo()]+ ", 0" + ", " + signalMap[this->outputs[0].returnNo()] + ", " + signalMap[this->outputs[1].returnNo()]+ ", " + signalMap[this->outputs[2].returnNo()] + ");";
    return out;
}

string Counter::returnVerilogCode(map<int, string>& signalMap, int ID)
{
    string out = "";
    if (this->inputs.size()==7)
        out = "  Counter U" + to_string(ID) + " (" + signalMap[this->inputs[0].returnNo()] + ", " + signalMap[this->inputs[1].returnNo()] + ", " + signalMap[this->inputs[2].returnNo()]+ ", " + signalMap[this->inputs[3].returnNo()]+ ", " + signalMap[this->inputs[4].returnNo()]+ ", " + signalMap[this->inputs[5].returnNo()]+ ", " + signalMap[this->inputs[6].returnNo()] + ", " + signalMap[this->outputs[0].returnNo()] + ", " + signalMap[this->outputs[1].returnNo()]+ ", " + signalMap[this->outputs[2].returnNo()] + ");";
    else
        out = "  Counter U" + to_string(ID) + " (" + signalMap[this->inputs[0].returnNo()] + ", " + signalMap[this->inputs[1].returnNo()] + ", " + signalMap[this->inputs[2].returnNo()]+ ", " + signalMap[this->inputs[3].returnNo()]+ ", " + signalMap[this->inputs[4].returnNo()]+ ", " + signalMap[this->inputs[5].returnNo()]+ ", 1b'0" + ", " + signalMap[this->outputs[0].returnNo()] + ", " + signalMap[this->outputs[1].returnNo()]+ ", " + signalMap[this->outputs[2].returnNo()] + ");";
    return out;
}


string XOR::returnVerilogCode(map<int, string>& signalMap, int ID)
{
    UNUSED(ID);
    string out = "  assign " + signalMap[this->outputs[0].returnNo()] + " = " + signalMap[this->inputs[0].returnNo()] + "^" + signalMap[this->inputs[1].returnNo()] + ";";
    return out;
}

string AND::returnVerilogCode(map<int, string>& signalMap, int ID)
{
    UNUSED(ID);
    string out = "  assign " + signalMap[this->outputs[0].returnNo()] + " = " + signalMap[this->inputs[0].returnNo()] + "&" + signalMap[this->inputs[1].returnNo()] + ";";
    return out;
}

string NAND::returnVerilogCode(map<int, string>& signalMap, int ID)
{
    UNUSED(ID);
    string out = "  assign " + signalMap[this->outputs[0].returnNo()] + " = " + "~(" + signalMap[this->inputs[0].returnNo()] + "&" + signalMap[this->inputs[1].returnNo()] + ");";
    return out;
}

string BUF::returnVerilogCode(map<int, string>& signalMap, int ID)
{
    UNUSED(ID);
    string out = "  assign " + signalMap[this->outputs[0].returnNo()] + " = " + signalMap[this->inputs[0].returnNo()] + ";";
    return out;
}

string CarryOperator::returnVerilogCode(map<int, string>& signalMap, int ID)
{
    UNUSED(ID);
    string out;
    if (this->inputs.size() == 2)
        out = "  assign " + signalMap[this->outputs[0].returnNo()] + " = " + signalMap[this->inputs[0].returnNo()] + "&" + signalMap[this->inputs[1].returnNo()] + ";";
    else if (this->inputs.size() == 3)
        out = "  assign " + signalMap[this->outputs[0].returnNo()] + " = " + signalMap[this->inputs[0].returnNo()] + "&" + signalMap[this->inputs[2].returnNo()] + ";\n" + "  assign " + signalMap[this->outputs[1].returnNo()] + " = " + signalMap[this->inputs[1].returnNo()] + "&" + signalMap[this->inputs[2].returnNo()] + ";";
    else 
        out = "  assign " + signalMap[this->outputs[0].returnNo()] + " = " + signalMap[this->inputs[0].returnNo()] + "&" + signalMap[this->inputs[2].returnNo()] + ";\n" + "  assign " + signalMap[this->outputs[1].returnNo()] + " = " + signalMap[this->inputs[3].returnNo()] + " | " + "(" + signalMap[this->inputs[1].returnNo()] + "&" + signalMap[this->inputs[2].returnNo()] + ");";
    return out;
}

string Skip::returnVerilogCode(map<int, string>& signalMap, int ID)
{
    UNUSED(ID);
    int ittr = this->inputs.size();
    string andChain = "";
    for (int i=1; i<ittr-1; i++) //for creating the and chain
    {
        andChain+=signalMap[this->inputs[i].returnNo()] + " & ";
    }
    andChain+=signalMap[this->inputs[ittr-1].returnNo()];

    string out = "  assign " + signalMap[this->outputs[0].returnNo()] + " = "+ signalMap[this->inputs[0].returnNo()]+ "| (" + andChain + ");\n";
    
    return out;
}

string ConstantOne::returnVerilogCode(map<int, string>& signalMap, int ID)
{
    string out = "  ConstatntOne U" + to_string(ID) + " (" + signalMap[this->outputs[0].returnNo()] + ");";
    return out;
}
