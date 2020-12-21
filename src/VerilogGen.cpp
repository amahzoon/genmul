#include "VerilogGen.hpp"

void GenerateMainHeader(int nIn1, int nIn2, string &file) //generate the header of module for main multiplier
{
    string moduleName = "module Mult_" + to_string(nIn1) + "_" + to_string(nIn2);
    string moduleParameters = "(IN1, IN2, Out);";
    string out = moduleName + moduleParameters;
    file += out + "\n";
}

void GenereateMainInOutSig(int nIn1, int nIn2, string &file) //generating input and output signals declaration for main Multiplier
{
    string in1Dec = "  input [" + to_string(nIn1 - 1) + ":0] IN1;";         //first input declaration
    string in2Dec = "  input [" + to_string(nIn2 - 1) + ":0] IN2;";         //second input declaration
    string outDec = "  output [" + to_string(nIn1 + nIn2 - 1) + ":0] Out;"; //output declaration
    file += in1Dec + "\n";
    file += in2Dec + "\n";
    file += outDec + "\n";
}

void GenereateMainWires(map<int, int> PPGInfo, int nOut1PPA, int nOut2PPA, int nAddOut, string &file) //generating input and output signals declaration for main Multiplier
{
    string s;
    for (auto i = 0u; i < PPGInfo.size(); i++)
    {
        s = "  wire [" + to_string(PPGInfo[i]-1) + ":0]" + " P" + to_string(i) + ";";
        file += s +"\n";
    }
    string Out1PPA = "  wire [" + to_string(nOut1PPA - 1) + ":0] R1;"; 
    string Out2PPA = "  wire [" + to_string(nOut2PPA - 1) + ":0] R2;"; 
    string OutFSA = "  wire [" + to_string(nAddOut - 1) + ":0] aOut;"; 

    file += Out1PPA +"\n";
    file += Out2PPA +"\n";
    file += OutFSA +"\n";
}

void GenerateHeader(int nIn1, int nIn2, string type, string &file) //generate the header of module for FSA
{
    string moduleName = "module " + type + "_" + to_string(nIn1) + "_" + to_string(nIn2);
    string moduleParameters = "(IN1, IN2, Out);";
    string out = moduleName + moduleParameters;
    file += out + "\n";
}
// Here nIn1b is the first number size with single bits
void GenerateComponentHeader(int nIn1, int nIn2, int nIn1b, int singleBitNum, string type, string &file) //generate the header for component instantioation (FSA)
{
    string componentName = "  " + type + "_" + to_string(nIn1) + "_" + to_string(nIn2) + " S2 ";
    string in1Range = "R1[" + to_string(nIn1b - 1) + ":" + to_string(singleBitNum) + "]";
    string outRange = "aOut[" + to_string(nIn1b) + ":" + to_string(singleBitNum) + "]";
    string componentParameters = "(" + in1Range + ", R2, " + outRange + ");";
    string out = componentName + componentParameters;
    file += out + "\n";
}

void GenerateHeader(int nIn1, int nIn2, int nOut, string type, string &file) //generate the header of module for PPG
{
    string moduleName = "module " + type + "_" + to_string(nIn1) + "_" + to_string(nIn2);
    string moduleParameters = "(IN1, IN2 ";
    for (int i = 0; i < nOut; i++)
    {
        moduleParameters += ", P" + to_string(i);
    }
    moduleParameters += ");";
    string out = moduleName + moduleParameters;
    file += out + "\n";
}

void GenerateComponentHeader(int nIn1, int nIn2, int nOut, string type, string &file) //generate the header for component instantioation (PPG)
{
    string componentName = "  " + type + "_" + to_string(nIn1) + "_" + to_string(nIn2) + " S0 ";
    string componentParameters = "(IN1, IN2 ";
    for (int i = 0; i < nOut; i++)
    {
        componentParameters += ", P" + to_string(i);
    }
    componentParameters += ");";
    string out = componentName + componentParameters;
    file += out + "\n";
}

void GenerateHeader(int nIn, string type, string &file) //generate the header of module for PPA
{
    string moduleName = "module " + type;
    string moduleParameters = "(";
    for (int i = 0; i < nIn; i++)
    {
        moduleParameters += "IN" + to_string(i) + ", ";
    }
    moduleParameters += "Out1, Out2);";
    string out = moduleName + moduleParameters;
    file += out + "\n";
}

void GenerateComponentHeader(int nIn, string type, string &file) //generate the header for component instantioation (PPA)
{
    string componentName = "  " + type + " S1 ";
    string componentParameters = "(";
    for (int i = 0; i < nIn; i++)
    {
        componentParameters += "P" + to_string(i) + ", ";
    }
    componentParameters += "R1, R2);";
    string out = componentName + componentParameters;
    file += out + "\n";
}

void GenereateInOutSig(int nIn1, int nIn2, int nOut, string &file) //generating input and output signals declaration FSA
{
    string in1Dec = "  input [" + to_string(nIn1 - 1) + ":0] IN1;";  //first input declaration
    string in2Dec = "  input [" + to_string(nIn2 - 1) + ":0] IN2;";  //second input declaration
    string outDec = "  output [" + to_string(nOut - 1) + ":0] Out;"; //output declaration
    file += in1Dec + "\n";
    file += in2Dec + "\n";
    file += outDec + "\n";
}

void GenereateInOutSig(int nIn1, int nIn2, map<int, int> Outs, string &file) //generating input and output signals declaration PPG
{
    string in1Dec = "  input [" + to_string(nIn1 - 1) + ":0] IN1;"; //first input declaration
    string in2Dec = "  input [" + to_string(nIn2 - 1) + ":0] IN2;"; //second input declaration
    vector<string> outDec;
    string singleOutDec;
    for (auto i = 0u; i < Outs.size(); i++)
    {
        singleOutDec = "  output [" + to_string(Outs[i] - 1) + ":0] P" + to_string(i) + ";";
        outDec.push_back(singleOutDec);
    }
    //string outDec = "  output [" + to_string(nOut - 1) + ":0] out;"; //output declaration
    file += in1Dec + "\n";
    file += in2Dec + "\n";
    for (auto i : outDec)
    {
        file += i + "\n";
    }
}

void GenereateInOutSig(map<int, int> Ins, int nOut1, int nOut2, string &file) //generating input and output signals declaration PPA
{
    string out1Dec = "  output [" + to_string(nOut1 - 1) + ":0] Out1;"; //first input declaration
    string out2Dec = "  output [" + to_string(nOut2 - 1) + ":0] Out2;"; //second input declaration
    vector<string> inDec;
    string singleInDec;
    for (auto i = 0u; i < Ins.size(); i++)
    {
        singleInDec = "  input [" + to_string(Ins[i] - 1) + ":0] IN" + to_string(i) + ";";
        inDec.push_back(singleInDec);
    }
    for (auto i : inDec)
    {
        file += i + "\n";
    }
    file += out1Dec + "\n";
    file += out2Dec + "\n";
}

map<int, string> generateWires(int nIn1, int nIn2, vector<int> signalIDs, vector<PartialProduct> outPartials, string &file) //generate wires and create an ID/name hash for them FSA
{
    vector<int> outputIDs;               //output signals IDs
    vector<int> wireIDs;                 //intermediate signals IDs
    map<int, int> mapIDweight; //a hash to store weights of outputs based on the IDs
    for (auto i : outPartials)
    {
        outputIDs.push_back(i.returnNo());
        mapIDweight[i.returnNo()] = i.returnWeight();
    }
    map<int, string> signalMap; //a hash to map signal IDs to real names
    for (auto i : signalIDs)
    {
        if (i < nIn1)
            signalMap[i] = "IN1[" + to_string(i) + "]";
        else if (i >= nIn1 && i < nIn1 + nIn2)
            signalMap[i] = "IN2[" + to_string(i - nIn1) + "]";
        else if (find(outputIDs.begin(), outputIDs.end(), i) != outputIDs.end()) //if the signal is an output signal
            signalMap[i] = "Out[" + to_string(mapIDweight[i]) + "]";
        else //if signal is an indermediate signal
        {
            signalMap[i] = "w" + to_string(i);
            wireIDs.push_back(i);
        }
    }
    //printing out the wire signal declaration
    string s;
    for (auto i : wireIDs)
    {
        s = "  wire " + signalMap[i] + ";";
        file += s + "\n";
    }
    return signalMap;
}

map<int, string> generateWires(int nIn1, int nIn2, vector<int> signalIDs, map<int, vector<PartialProduct>> outPartials, string &file) //generate wires and create an ID/name hash for them PPG
{
    map<int, int> outputIDsMap; //Map the partial no to the wight of the partial:: No->weight
    vector<int> outputIDs;                //for storing all output IDs
    vector<int> wireIDs;                  //intermediate signals IDs
    map<int, int> mapIDweight;  //a hash to store weights of outputs based on the IDs
    for (auto i = 0u; i < outPartials.size(); i++)
    {
        for (auto j : outPartials[i])
        {
            outputIDsMap[j.returnNo()] = i;
            outputIDs.push_back(j.returnNo());
        }
    }
    map<int, int> track;        //to keep track of the output numbers with the same weights --> Px[?]
    map<int, string> signalMap; //a hash to map signal IDs to real names
    for (auto i : signalIDs)
    {
        if (i < nIn1)
            signalMap[i] = "IN1[" + to_string(i) + "]";
        else if (i >= nIn1 && i < nIn1 + nIn2)
            signalMap[i] = "IN2[" + to_string(i - nIn1) + "]";
        else if (find(outputIDs.begin(), outputIDs.end(), i) != outputIDs.end()) //if the signal is an output signal
        {
            signalMap[i] = "P" + to_string(outputIDsMap[i]) + "[" + to_string(track[outputIDsMap[i]]++) + "]";
        }
        else //if signal is an indermediate signal
        {
            signalMap[i] = "w" + to_string(i);
            wireIDs.push_back(i);
        }
    }
    //printing out the wire signal declaration
    string s;
    for (auto i : wireIDs)
    {
        s = "  wire " + signalMap[i] + ";";
        file += s + "\n";
    }
    return signalMap;
}

map<int, string> generateWires(map<int, int> Ins, vector<int> signalIDs, vector<PartialProduct> OutPar1, vector<PartialProduct> OutPar2, string &file) //generate wires and create an ID/name hash for them PPG
{
    map<int, int> inputIDsMap;  //Map the input no to the weight of it
    map<int, int> inputIDsMap2; //Map the input no to the location in a certain weight
    vector<int> output1IDs;               //output1 signals IDs
    vector<int> output2IDs;               //output2 signals IDs
    vector<int> wireIDs;                  //intermediate signals IDs
    map<int, int> mapIDOut1;
    map<int, int> mapIDOut2;

    int count = 0;
    for (auto i = 0u; i < Ins.size(); i++)
    {
        for (int j = 0; j < Ins[i]; j++)
        {
            inputIDsMap[count] = i;
            inputIDsMap2[count] = j;
            count++;
        }
    }

    for (auto i = 0u; i < OutPar1.size(); i++)
    {
        output1IDs.push_back(OutPar1[i].returnNo());
        mapIDOut1[OutPar1[i].returnNo()] = i;
    }
    for (auto i = 0u; i < OutPar2.size(); i++)
    {
        output2IDs.push_back(OutPar2[i].returnNo());
        mapIDOut2[OutPar2[i].returnNo()] = i;
    }

    map<int, string> signalMap; //a hash to map signal IDs to real names
    for (auto i : signalIDs)
    {
        if (i < count)
            signalMap[i] = "IN" + to_string(inputIDsMap[i]) + "[" + to_string(inputIDsMap2[i]) + "]";
        else if (find(output1IDs.begin(), output1IDs.end(), i) != output1IDs.end())
        {
            signalMap[i] = "Out1[" + to_string(mapIDOut1[i]) + "]";
        }
        else if (find(output2IDs.begin(), output2IDs.end(), i) != output2IDs.end())
        {
            signalMap[i] = "Out2[" + to_string(mapIDOut2[i]) + "]";
        }
        else //if signal is an indermediate signal
        {
            signalMap[i] = "w" + to_string(i);
            wireIDs.push_back(i);
        }
    }
    //printing out the wire signal declaration
    string s;
    for (auto i : wireIDs)
    {
        s = "  wire " + signalMap[i] + ";";
        file += s + "\n";
    }
    return signalMap;
}

void GenerateComponents(map<int, string>& signalMap, vector<Component *>& compList, string &file)
{
    string s;
    int ComponentID = 0; //an ID which is needed for components mapping
    string temp = "";
    for (auto &i : compList)
    {
        s = i->returnVerilogCode(signalMap, ComponentID);
        temp = temp+s+"\n";
        ComponentID++;
    }
    file += temp + "\n";
}

void GeneratePredictors(map<int, string> signalMap, vector<CarryPredictor *> predictorList, string &file)
{
    string s;
    for (auto i : predictorList)
    {
        s = i->returnVerilogCode(signalMap);
        file += s + "\n";
    }
}

void GenerateFullAdder (string &file)
{
  file +="module FullAdder(X, Y, Z, S, C);\n";
  file +="  output C;\n";
  file +="  output S;\n";
  file +="  input X;\n";
  file +="  input Y;\n";
  file +="  input Z;\n";
  file +="  assign C = ( X & Y ) | ( Y & Z ) | ( Z & X );\n";
  file +="  assign S = X ^ Y ^ Z;\n";
file +="endmodule\n";
}

void GenerateHalfAdder (string &file)
{
  file +="module HalfAdder(X, Y, S, C);\n";
  file +="  output C;\n";
  file +="  output S;\n";
  file +="  input X;\n";
  file +="  input Y;\n";
  file +="  assign C = X & Y;\n";
  file +="  assign S = X ^ Y;\n";
  file +="endmodule\n";
}

void GenerateFullAdderProp (string &file)
{
  file +="module FullAdderProp(X, Y, Z, S, C, P);\n";
  file +="  output C;\n";
  file +="  output S;\n";
  file +="  output P;\n";
  file +="  input X;\n";
  file +="  input Y;\n";
  file +="  input Z;\n";
  file +="  assign C = ( X & Y ) | ( Y & Z ) | ( Z & X );\n";
  file +="  assign S = X ^ Y ^ Z;\n";
  file +="  assign P = X ^ Y;\n";
  file +="endmodule\n";
}

void GenerateConstantOne (string &file)
{
  file +="module ConstatntOne(O);\n";
  file +="  output O;\n";
  file +="  assign O = 1;\n";
  file +="endmodule\n";
}

void GenerateCompressor (string &file)
{
  file +="module Compressor(IN0, IN1, IN2, IN3, Ci, S, C, Co);\n";
  file +="output C;\n";
  file +="output Co;\n";
  file +="output S;\n";
  file +="input Ci;\n";
  file +="input IN0;\n";
  file +="input IN1;\n";
  file +="input IN2;\n";
  file +="input IN3;\n";
  file +="wire W0;\n";
  file +="wire W1;\n";
  file +="wire W2;\n";
  file +="assign W0 = IN0 ^ IN1;\n";
  file +="assign W1 = IN2 ^ IN3;\n";
  file +="assign W2 = W0 ^ W1;\n";
  file +="assign S = W2 ^ Ci;\n";
  file +="assign C = ( W2 & Ci ) | ( ~ W2 & IN3 );\n";
  file +="assign Co = ( W0 & IN2 ) | ( ~ W0 & IN0 );\n";
  file +="endmodule\n";
}


void GenerateCounter (string &file)
{
  file +="module Counter(X1, X2, X3, X4, X5, X6, X7, S3, S2, S1);\n";
  file +="output S1;\n";
  file +="output S2;\n";
  file +="output S3;\n";
  file +="input X1;\n";
  file +="input X2;\n";
  file +="input X3;\n";
  file +="input X4;\n";
  file +="input X5;\n";
  file +="input X6;\n";
  file +="input X7;\n";
  file +="wire W1;\n";
  file +="wire W2;\n";
  file +="wire W3;\n";
  file +="wire W4;\n";
  file +="wire W5;\n";
  file +="wire W6;\n";
  file +="assign W1 = X1 ^ X2 ^ X3;\n";
  file +="assign W2 = X4 ^ X5 ^ ( X6 ^ X7 );\n";
  file +="assign W3 = ~ ( ( ~ ( X1 & X2 ) ) & ( ~ ( X1 & X3 ) ) & ( ~ ( X2 & X3 ) ) );\n";
  file +="assign W4 = ~ ( ( ~ ( ( X4 | X5 ) & ( X6 | X7 ) ) ) & ( ~ ( ( X4 & X5 ) | ( X6 & X7 ) ) ) );\n";
  file +="assign W5 = ~ ( X4 & X5 & X6 & X7 );\n";
  file +="assign W6 = ~ ( ( ~ ( W4 & W5 ) ) ^ W3 );\n";
  file +="assign S3 = W1 ^ W2;\n";
  file +="assign S2 = ~ ( W6 ^ ( ~ ( W1 & W2 ) ) );\n";
  file +="assign S1 = ~ ( W5 & ( ~ ( W3 & W4 ) ) & ( ~ ( W1 & W2 & W6 ) ) );\n";
  file +="endmodule\n";
}


string infoPrint (int firstInputLength, int secondInputLength, int firstStageID, int secondStageID, int thirdStageID)
{
    string info = "";
    string firstStage, secondStage, thirdStage;
    switch (firstStageID)
    {
        case 1: firstStage = "Unsigned simple partial product generator [U_SP]";
            break;
        case 2: firstStage = "Signed simple partial product generator [S_SP]";
            break;
    }
    switch (secondStageID)
    {
        case 1: secondStage = "Array [AR]";
            break;
        case 2: secondStage = "Wallace tree [WT]";
            break;
        case 3: secondStage = "Dadda tree [DT]";
            break;
        case 4: secondStage = "Counter-based Wallace tree [CWT]";
            break;
    }
    switch (thirdStageID)
    {
        case 1: thirdStage = "Ripple carry adder [RC]";
            break;
        case 2: thirdStage = "Carry look-ahead adder [CL]";
            break;
        case 3: thirdStage = "Lander-Fischer adder [LF]";
            break;
        case 4: thirdStage = "Kogge-Stone adder [KS]";
            break;
        case 5: thirdStage = "Brent-Kung adder [BK]";
            break;
        case 6: thirdStage = "Carry skip adder [CS]";
            break;
        case 7: thirdStage = "Serial prefix adder [SE]";
            break;
    }
    info+="/*----------------------------------------------------------------------------\n";
    info+="Copyright (c) 2019-2020 University of Bremen, Germany.\n";
    info+="Copyright (c) 2020 Johannes Kepler University Linz, Austria.\n";
    info+="This file has been generated with GenMul.\n";
    info+="You can find GenMul at: http://www.sca-verification.org/genmul\n";
    info+="Contact us at genmul@sca-verification.org\n\n";
    info+="  First input length: "+to_string(firstInputLength)+"\n";
    info+="  second input length: "+to_string(secondInputLength)+"\n";
    info+="  Partial product generator: "+firstStage+"\n";
    info+="  Partial product accumulator: "+secondStage+"\n";
    info+="  Final stage adder: "+thirdStage+"\n";
    info+="----------------------------------------------------------------------------*/\n";
    return info;
}
