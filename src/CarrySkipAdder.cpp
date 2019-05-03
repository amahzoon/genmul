#include "CarrySkipAdder.hpp"

int CarrySkipAdder(int nIn1, int nIn2, string &file) // Get two integer numbers as input sizes and create the ripple carry adder
{
    assert(nIn1 > 0 && nIn2 > 0 && "Lenght of input bits should be bigger than zero");

    vector<PartialProduct> partialIn1 = PartialProduct::createPartialArray(nIn1); //create an array of partial products for the first input
    vector<PartialProduct> partialIn2 = PartialProduct::createPartialArray(nIn2); //create an array of partial products for the second input
    vector<PartialProduct> partialOut;                                            //the array of partial products for the output
    vector<Component *> compList;                                                 //the array of used components during implementation

    map<int, vector<PartialProduct>> LevelizedPartials; //for hashing partial products based on the weights

    PartialProduct::LevelizePartials(LevelizedPartials, partialIn1); //adding the first input to the levelized hash
    PartialProduct::LevelizePartials(LevelizedPartials, partialIn2); //adding the second input to the levelized hash

    //we should first calculate the optimal size for carry skip length
    int nInMax = (nIn1 >= nIn2) ? nIn1 : nIn2;
    int optimalSkipSize = sqrt((double)nInMax / 2);
    int skipNumbers = (nInMax / optimalSkipSize);
    int initialSkip = nInMax - skipNumbers * optimalSkipSize;
    if (initialSkip == 0) //because the input carry is zero we should surely ignore some blocks at the first stages
        initialSkip = optimalSkipSize;
    int curentWeight = 0;
    bool endFlag = false; // a flag which indicates the end of the implementation

    Component *comp; //a temp component to store a just created component

    int counter = 0;
    vector<PartialProduct> propagteVec;
    vector<PartialProduct> CinOutSkip; //the carry in and carry out of the Skip block (first:Cin,  second:Cout)

    while (!endFlag)
    {
        if (LevelizedPartials[curentWeight].size() == 2) //if there are two partial products with the same weights
        {
            comp = new HalfAdder(LevelizedPartials[curentWeight]);
            LevelizedPartials[curentWeight].clear();
            comp->SetOutputs();
            if (initialSkip != 0)
                initialSkip--;
            else
            {
                propagteVec.push_back(comp->returnOutputs()[0]);
                counter++;
            }
        }
        else if (initialSkip != 0)
        {
            comp = new FullAdder(LevelizedPartials[curentWeight]);
            LevelizedPartials[curentWeight].clear();
            comp->SetOutputs();
            initialSkip--;
        }
        else
        {
            counter++;
            comp = new FullAdderProp(LevelizedPartials[curentWeight]);
            LevelizedPartials[curentWeight].clear();
            comp->SetOutputs();
            propagteVec.push_back(comp->returnOutputs()[2]);
        }
        if (counter == 1)
        {
            propagteVec.push_back(comp->returnInputs()[2]);
        }
        if (counter == optimalSkipSize)
        {
            compList.push_back(comp);
            PartialProduct::LevelizePartials(LevelizedPartials, {comp->returnOutputs()[0]});
            propagteVec.insert(propagteVec.begin(), comp->returnOutputs()[1]);
            comp = new Skip(propagteVec);
            comp->SetOutputs();
            counter = 0;
            propagteVec.clear();
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
    GenerateHeader(nIn1, nIn2, "CK", file);
    GenereateInOutSig(nIn1, nIn2, partialOut.size(), file);
    vector<int> signalIDs = Component::collectIDs(compList); //all signals IDs
    map<int, string> wireHash = generateWires(nIn1, nIn2, signalIDs, partialOut, file);
    GenerateComponents(wireHash, compList, file);
    file += "endmodule\n";
    return partialOut.size();
}
