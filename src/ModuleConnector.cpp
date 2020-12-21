#include "ModuleConnector.hpp"

//first stage => 1: Simple PPG (unsigned)    2:simple PPG (signed)
//second stage => 1: Array    2:Wallace   3: Dadda
//third stage => 1:Ripple carry   2: Carry look-ahead   3: Lander-Fischer   4: Kogge-Stone  5: Brent-Kung  6: Carry Skip Adder

int PartialProduct::count = 0;

string moduleConnector(int nIn1, int nIn2, int firstStage, int secondStage, int thirdStage) // Connect three stages to create a multiplier
{
    assert(firstStage >= 1 && firstStage <= 2 && "The assigned number for the first stage should be 1 or 2!");
    assert(secondStage >= 1 && secondStage <= 4 && "The assigned number for the second stage should be 1, 2, 3, or 4!");
    assert(thirdStage >= 1 && thirdStage <= 7 && "The assigned number for the third stage should be 1, 2, 3, 4, 5, 6, or 7!");
    string firstStageName, secondStageName, thirdStageName;

    string file = ""; //the content of the final verilog file
    //adding compilation version
    file+="//Compilation time: "+GLOBAL_COMPILATION_TIME+"\n";
    file+="//Compilation SHA256 message digest: "+GLOBAL_SHA256+"\n";
    //Adding the multiplier info
    file+=infoPrint (nIn1, nIn2, firstStage, secondStage, thirdStage);
    //Generating some basic modules
    GenerateFullAdder(file);
    GenerateFullAdderProp(file);
    GenerateHalfAdder(file);
    GenerateConstantOne(file);
    GenerateCounter(file);
    //
    //Implementing the first stage of multiplier
    PartialProduct::SetCountZero();
    bool sign;
    map<int, int> PPGInfo;
    switch (firstStage)
    {
    case 1:
        PPGInfo = PartialProductGenerator(nIn1, nIn2, file);
        firstStageName = "U_SP";
        sign = false;
        break;
    case 2:
        PPGInfo = PartialProductGeneratorSigned(nIn1, nIn2, file);
        firstStageName = "S_SP";
        sign = true;
        break;
    }
    //cout << "Partial Product Generartion: DONE" << endl;
    PartialProduct::SetCountZero(); //to refresh all IDs
    /////////////////////////////////////////////////////
    //Implementing the second stage of multiplier
    vector<int> PPAInfo;
    switch (secondStage)
    {
    case 1:
        PPAInfo = Array(PPGInfo, nIn1, nIn2, file, sign);
        secondStageName = "AR";
        break;
    case 2:
        PPAInfo = Wallace(PPGInfo, nIn1, nIn2, file);
        secondStageName = "WT";
        break;
    case 3:
        PPAInfo = Dadda(PPGInfo, nIn1, nIn2, file);
        secondStageName = "DT";
        break;
    case 4:
        PPAInfo = Wallace5(PPGInfo, nIn1, nIn2, file);
        secondStageName = "CWT";
        break;
    }
    //cout << "Partial Product accumulation: DONE" << endl;
    PartialProduct::SetCountZero();
    /////////////////////////////////////////////////////
    //Implementing the third stage of multiplier
    int nAdd;
    switch (thirdStage)
    {
    case 1:
        nAdd = CreateRippleCarryAdder(PPAInfo[0] - PPAInfo[2], PPAInfo[1], file);
        thirdStageName = "RC";
        break;
    case 2:
        nAdd = CarryLookAhead(PPAInfo[0] - PPAInfo[2], PPAInfo[1], file);
        thirdStageName = "CL";
        break;
    case 3:
        nAdd = LanderFischerAdder(PPAInfo[0] - PPAInfo[2], PPAInfo[1], file);
        thirdStageName = "LF";
        break;
    case 4:
        nAdd = KoggeStoneAdder(PPAInfo[0] - PPAInfo[2], PPAInfo[1], file);
        thirdStageName = "KS";
        break;
    case 5:
        nAdd = BrentKungAdder(PPAInfo[0] - PPAInfo[2], PPAInfo[1], file);
        thirdStageName = "BK";
        break;
    case 6:
        nAdd = CarrySkipAdder(PPAInfo[0] - PPAInfo[2], PPAInfo[1], file);
        thirdStageName = "CK";
        break;
    case 7:
        nAdd = SerialPrefixAdder(PPAInfo[0] - PPAInfo[2], PPAInfo[1], file);
        thirdStageName = "SE";
        break;
    }
    //cout << "Final Stage Adder: DONE" << endl;
    /////////////////////////////////////////////////////
    //Generating the main Multiplier module
    GenerateMainHeader(nIn1, nIn2, file);
    GenereateMainInOutSig(nIn1, nIn2, file);
    GenereateMainWires(PPGInfo, PPAInfo[0], PPAInfo[1], nAdd + PPAInfo[2], file);

    GenerateComponentHeader(nIn1, nIn2, PPGInfo.size(), firstStageName, file);
    GenerateComponentHeader(PPGInfo.size(), secondStageName, file);
    GenerateComponentHeader(PPAInfo[0] - PPAInfo[2], PPAInfo[1], PPAInfo[0], PPAInfo[2], thirdStageName, file);

    string s;
    for (int i = 0; i < PPAInfo[2]; i++)
    {
        s = "  assign aOut[" + to_string(i) + "] = R1[" + to_string(i) + "];";
        file += s + "\n";
    }
    s = "  assign Out = aOut[" + to_string(nIn1 + nIn2 - 1) + ":0];";
    file += s + "\n";
    file += "endmodule\n";

    file += "\n";
    file += "/*---------------------------------------------------------------------------------------------------\n";
    file += "This are SHA256 message digests computed for all source files to see the version of a file in genmul.\n";
    file += GLOBAL_SHA256_DETAILED;
    file += "---------------------------------------------------------------------------------------------------*/\n";

    return file;
}

string nameMaker (int nIn1, int nIn2, int firstStage, int secondStage, int thirdStage) //create name for the final Verilog file
{
    assert(firstStage >= 1 && firstStage <= 2 && "The assigned number for the first stage should be 1 or 2!");
    assert(secondStage >= 1 && secondStage <= 4 && "The assigned number for the second stage should be 1, 2, 3, or 4!");
    assert(thirdStage >= 1 && thirdStage <= 7 && "The assigned number for the third stage should be 1, 2, 3, 4, 5, 6, or 7!");
    string firstStageName, secondStageName, thirdStageName;

    string name;

    switch (firstStage)
    {
    case 1:
        firstStageName = "U_SP";
        break;
    case 2:
        firstStageName = "S_SP";
        break;
    }
    /////////////////////////////////////////////////////
    switch (secondStage)
    {
    case 1:
        secondStageName = "AR";
        break;
    case 2:
        secondStageName = "WT";
        break;
    case 3:
        secondStageName = "DT";
        break;
    case 4:
        secondStageName = "CWT";
        break;
    }
    /////////////////////////////////////////////////////
    switch (thirdStage)
    {
    case 1:
        thirdStageName = "RC";
        break;
    case 2:
        thirdStageName = "CL";
        break;
    case 3:
        thirdStageName = "LF";
        break;
    case 4:
        thirdStageName = "KS";
        break;
    case 5:
        thirdStageName = "BK";
        break;
    case 6:
        thirdStageName = "CK";
        break;
    case 7:
        thirdStageName = "SE";
        break;
    }

    name = to_string(nIn1) + "_" + to_string(nIn2) + "_" + firstStageName + "_" + secondStageName + "_" + thirdStageName + "_" + "GenMul.v";
    return name;

}
