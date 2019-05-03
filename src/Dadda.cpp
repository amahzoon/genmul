#include "Dadda.hpp"

#define UNUSED(x) (void)(x)

vector<int> Dadda(map<int, int> Ins, int nIn1, int nIn2, string &file) // Get two integer numbers as input sizes and create the Wallace Tree PPA
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

    //we should find the maximum hight of the partial product tree
    unsigned int maxh = 0; //h will show the max height
    for (auto i: LevelizedPartials)
    {
        if (i.second.size()>maxh)
            maxh = i.second.size();
    }

    int d = 2;           //d parameter in dadda algorithm
    vector<int> D = {d}; //a vector to store all possible d parameters

    bool stopFlag = false;      //a flag to determine when the loop should be stoped

    while (!stopFlag)
    {
        d = 1.5 * d;
        D.push_back(d);
        //if ((1.5 * d) > nIn1 || (1.5 * d) > nIn2)
        if ((1.5 * d) > maxh)
            stopFlag = true;
    }
    reverse(D.begin(), D.end());

    map<int, int> height; //to keep the track of the heights during dadda reduction

    for (auto i : LevelizedPartials)
    {
        height[i.first] = i.second.size();
    }

    vector<PartialProduct> GeneratedAtLevel; //all partial products generated a level of wallce tree computations
    for (auto n = 0u; n < D.size(); n++)
    {
        for (auto i = 0u; i < LevelizedPartials.size(); i++)
        {
            while (1)
            {
                if (height[i] <= D[n])
                {
                    break;
                }
                else if (height[i] == D[n] + 1)
                {
                    height[i]--;
                    height[i + 1]++;
                    comp = new HalfAdder({LevelizedPartials[i][0], LevelizedPartials[i][1]});
                    comp->SetOutputs();
                    LevelizedPartials[i].erase(LevelizedPartials[i].begin(), LevelizedPartials[i].begin() + 2);
                    compList.push_back(comp);
                    GeneratedAtLevel.push_back(comp->returnOutputs()[0]);
                    GeneratedAtLevel.push_back(comp->returnOutputs()[1]);
                    break;
                }
                else
                {
                    height[i] -= 2;
                    height[i + 1]++;
                    comp = new FullAdder({LevelizedPartials[i][0], LevelizedPartials[i][1], LevelizedPartials[i][2]});
                    comp->SetOutputs();
                    LevelizedPartials[i].erase(LevelizedPartials[i].begin(), LevelizedPartials[i].begin() + 3);
                    compList.push_back(comp);
                    GeneratedAtLevel.push_back(comp->returnOutputs()[0]);
                    GeneratedAtLevel.push_back(comp->returnOutputs()[1]);
                }
            }
        }
        PartialProduct::LevelizePartials(LevelizedPartials, GeneratedAtLevel);
        GeneratedAtLevel.clear();
    }

    //Some of the input partial product needs to go directely to output, so we connect them with a = b assignment!
    for (auto &i : LevelizedPartials)
    {
        for (auto &j : i.second)
        {
            if (j.returnNo() < inputNumber)
            {
                comp = new BUF({j});
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
    for (auto i = 0u; i < LevelizedPartials.size(); i++)
    {
        if (LevelizedPartials[i].size() == 1)
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
    GenerateHeader(Ins.size(), "DT", file);
    GenereateInOutSig(Ins, nOut1, nOut2, file);
    vector<int> signalIDs = Component::collectIDs(compList); //all signals IDs
    map<int, string> wireHash = generateWires(Ins, signalIDs, OutPar1, OutPar2, file);
    GenerateComponents(wireHash, compList, file);
    file += "endmodule\n";
    return {(int)OutPar1.size(), (int)OutPar2.size(), singleBitNumber}; //the first and second members are the size of out1 and out2, respectively, and the third output is the number pf single bits at the begining
}
