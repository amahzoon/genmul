#include "RippleCarryAdder.hpp"

int CreateRippleCarryAdder(int nIn1, int nIn2, string &file) // Get two integer numbers as input sizes and create the ripple carry adder
{
    assert(nIn1 > 0 && nIn2 > 0 && "Lenght of input bits should be bigger than zero");

    vector<PartialProduct> partialIn1 = PartialProduct::createPartialArray(nIn1); //create an array of partial products for the first input
    vector<PartialProduct> partialIn2 = PartialProduct::createPartialArray(nIn2); //create an array of partial products for the second input
    vector<PartialProduct> partialOut;                                            //the array of partial products for the output
    vector<Component *> compList;                                                 //the array of used components during implementation

    map<int, vector<PartialProduct>> LevelizedPartials; //for hashing partial products based on the weights

    PartialProduct::LevelizePartials(LevelizedPartials, partialIn1); //adding the first input to the levelized hash
    PartialProduct::LevelizePartials(LevelizedPartials, partialIn2); //adding the second input to the levelized hash

    int curentWeight = 0;
    bool endFlag = false; // a flag which indicates the end of the implementation

    Component *comp; //a temp component to store a just created component

    while (!endFlag)
    {
        if (LevelizedPartials[curentWeight].size() == 2) //if there are two partial products with the same weights
        {
            comp = new HalfAdder(LevelizedPartials[curentWeight]);
            LevelizedPartials[curentWeight].clear();
            comp->SetOutputs();
        }
        else //if there are three partial products with the same weights
        {
            comp = new FullAdder(LevelizedPartials[curentWeight]);
            LevelizedPartials[curentWeight].clear();
            comp->SetOutputs();
        }
        PartialProduct::LevelizePartials(LevelizedPartials, comp->returnOutputs());
        compList.push_back(comp);

        curentWeight++;
        if (LevelizedPartials[curentWeight].size() < 2)
            endFlag = true;
    }

    for (auto i = 0u; i < LevelizedPartials.size(); i++) //creating the list of outputs
    {
        partialOut.push_back(LevelizedPartials[i][0]);
    }

    //Generating the output verilog file
    GenerateHeader(nIn1, nIn2, "RC", file);
    GenereateInOutSig(nIn1, nIn2, partialOut.size(), file);
    vector<int> signalIDs = Component::collectIDs(compList); //all signals IDs
    map<int, string> wireHash = generateWires(nIn1, nIn2, signalIDs, partialOut, file);
    GenerateComponents(wireHash, compList, file);
    file += "endmodule\n";
    return partialOut.size();
}
