#include "Array.hpp"

vector<int> Array(map<int, int> Ins, int nIn1, int nIn2, string &file, bool sign) // Get two integer numbers as input sizes and create the Array PPA
{
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

    int in1Size, in2Size, range;

    if (sign == false) //if multiplier is unsigned
    {
        in1Size = nIn1;
        in2Size = nIn2;
        range = in1Size - 1;
    }
    else //if multiplier is signed we have done a sign extention so the sizes are now equal
    {
        if (nIn1>nIn2)
        {
            in1Size = nIn1;
            in2Size = nIn1;
        }
        else
        {
            in1Size = nIn2;
            in2Size = nIn2;
        }
        range = in1Size;
    }

    map<int, vector<PartialProduct>> LevelizedPartials; //for hashing partial products based on the weights

    PartialProduct::LevelizePartials(LevelizedPartials, partialIn); //adding the first input to the levelized hash

    Component *comp; //a temp component to store a just created component

    

    int currentWeight = 1;

    for (int i = 0; i < in2Size - 1; i++) //determining the number of rows
    {
        comp = new HalfAdder({LevelizedPartials[currentWeight][0], LevelizedPartials[currentWeight][1]});
        LevelizedPartials[currentWeight].erase(LevelizedPartials[currentWeight].begin(), LevelizedPartials[currentWeight].begin() + 2); //removing added partial products
        comp->SetOutputs();
        LevelizedPartials[currentWeight].insert(LevelizedPartials[currentWeight].begin(), comp->returnOutputs()[0]); //adding output of HA to the list of partial products
        LevelizedPartials[currentWeight + 1].insert(LevelizedPartials[currentWeight + 1].begin(), comp->returnOutputs()[1]);
        compList.push_back(comp);
        for (int j = 1; j < range; j++)
        {
            comp = new FullAdder({LevelizedPartials[currentWeight + j][0], LevelizedPartials[currentWeight + j][1], LevelizedPartials[currentWeight + j][2]});
            LevelizedPartials[currentWeight + j].erase(LevelizedPartials[currentWeight + j].begin(), LevelizedPartials[currentWeight + j].begin() + 3); //removing added partial products
            comp->SetOutputs();
            LevelizedPartials[currentWeight + j].insert(LevelizedPartials[currentWeight + j].begin(), comp->returnOutputs()[0]); //adding output of FA to the list of partial products
            LevelizedPartials[currentWeight + j + 1].insert(LevelizedPartials[currentWeight + j + 1].begin(), comp->returnOutputs()[1]);
            compList.push_back(comp);
        }
        currentWeight++;
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
    GenerateHeader(Ins.size(), "AR", file);
    GenereateInOutSig(Ins, nOut1, nOut2, file);
    vector<int> signalIDs = Component::collectIDs(compList); //all signals IDs
    map<int, string> wireHash = generateWires(Ins, signalIDs, OutPar1, OutPar2, file);
    GenerateComponents(wireHash, compList, file);
    file += "endmodule\n";
    return {(int)OutPar1.size(), (int)OutPar2.size(), singleBitNumber}; //the first and second members are the size of out1 and out2, respectively, and the third output is the number pf single bits at the begining
}
