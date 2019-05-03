#include "Wallace.hpp"

#define UNUSED(x) (void)(x)
vector<int> Wallace(map<int, int> Ins, int nIn1, int nIn2, string &file) // Get two integer numbers as input sizes and create the Wallace Tree PPA
{
    UNUSED(nIn1);
    UNUSED(nIn2);
    int inputNumber = 0;
    vector<PartialProduct> partialIn; //to store all partial products
    for (auto i = 0u; i < Ins.size(); i++)
    {
        for (int j = 0; j < Ins[i]; j++)
        {
            PartialProduct p(i);
            partialIn.push_back(p);
            inputNumber++;
        }
    }

    vector<PartialProduct> partialOut; //the array of partial products for the output
    vector<Component *> compList;      //the array of used components during implementation

    map<int, vector<PartialProduct>> LevelizedPartials; //for hashing partial products based on the weights

    PartialProduct::LevelizePartials(LevelizedPartials, partialIn); //adding the first input to the levelized hash

    Component *comp; //a temp component to store a just created component


    bool stopFlag = false; //a flag to determine when the loop should be stoped
    vector<PartialProduct> GeneratedAtLevel; //all partial products generated a level of wallce tree computations
    int iterationNum = 0;
    while (!stopFlag)
    {
        for (auto &i: LevelizedPartials)
        {
            iterationNum = (i.second.size())/3;
            for (int n=0; n<iterationNum; n++)
            {
                comp = new FullAdder ({i.second[0], i.second[1], i.second[2]});
                comp->SetOutputs();
                i.second.erase(i.second.begin(), i.second.begin()+3);
                compList.push_back(comp);
                GeneratedAtLevel.push_back(comp->returnOutputs()[0]);
                GeneratedAtLevel.push_back(comp->returnOutputs()[1]);
            }
            if (i.second.size()==2)
            {
                comp = new HalfAdder ({i.second[0], i.second[1]});
                comp->SetOutputs();
                i.second.erase(i.second.begin(), i.second.begin()+2);
                compList.push_back(comp);
                GeneratedAtLevel.push_back(comp->returnOutputs()[0]);
                GeneratedAtLevel.push_back(comp->returnOutputs()[1]);
            }
        }
        PartialProduct::LevelizePartials(LevelizedPartials, GeneratedAtLevel);
        GeneratedAtLevel.clear();
        stopFlag = true;
        for (auto i: LevelizedPartials)
        {
            if (i.second.size()>=3)
            {
                stopFlag = false;
                break;
            }
        }
    }



    //Some of the input partial product needs to go directely to output, so we connect them with a = b assignment!
    for (auto &i : LevelizedPartials)
    {
        for (auto &j : i.second)
        {
            if (j.returnNo()<inputNumber)
            {
                comp = new BUF ({j});
                comp->SetOutputs();
                j = comp->returnOutputs()[0];
                compList.push_back(comp);
            }
        }
    }
    ////////////////////

    vector<PartialProduct> OutPar1; //this output is always has more bits
    vector<PartialProduct> OutPar2;
    //for determining the size of the two outputs
    int singleBitNumber = 0;
    int nOut1 = 0;
    int nOut2 = 0;
    for (auto i : LevelizedPartials)
    {
        if (i.second.size() == 1)
            nOut1++;
        else if (i.second.size() == 2)
        {
            nOut1++;
            nOut2++;
        }
    }

    //to count the number of the single output that should directely go to the final output
    for (auto i=0u; i<LevelizedPartials.size();i++)
    {
        if (LevelizedPartials[i].size()==1)
            singleBitNumber++;
        else
            break;
    }
    //for Collecting two final numbers as outputs
    for (auto i = 0u; i < LevelizedPartials.size(); i++)
    {
        if (LevelizedPartials[i].size() == 1)
            OutPar1.push_back(LevelizedPartials[i][0]);
        if (LevelizedPartials[i].size() == 2)
        {
            OutPar1.push_back(LevelizedPartials[i][0]);
            OutPar2.push_back(LevelizedPartials[i][1]);
        }
    }
    /////////////////////

    //Generating the output verilog file
    GenerateHeader(Ins.size(), "WT", file);
    GenereateInOutSig(Ins, nOut1, nOut2, file);
    vector<int> signalIDs = Component::collectIDs(compList); //all signals IDs
    map<int, string> wireHash = generateWires(Ins, signalIDs, OutPar1, OutPar2, file);
    GenerateComponents(wireHash, compList, file);
    file += "endmodule\n";
    return {(int)OutPar1.size(), (int)OutPar2.size(), singleBitNumber}; //the first and second members are the size of out1 and out2, respectively, and the third output is the number pf single bits at the begining
}
