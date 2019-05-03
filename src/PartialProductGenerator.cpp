#include "PartialProductGenerator.hpp"

map<int, int> PartialProductGenerator(int nIn1, int nIn2, string &file) // Get two integer numbers as input sizes and return the levelized generated partial products
{
    assert(nIn1 > 0 && nIn2 > 0 && "Lenght of input bits should be bigger than zero");

    vector<PartialProduct> partialIn1 = PartialProduct::createPartialArray(nIn1); //create an array of partial products for the first input
    vector<PartialProduct> partialIn2 = PartialProduct::createPartialArray(nIn2); //create an array of partial products for the second input
    vector<PartialProduct> generatedPartials;

    Component *comp;              //a temp component to store a just created component
    vector<Component *> compList; //the array of used components during implementation

    map<int, vector<PartialProduct>> LevelizedPartials; //for hashing partial products based on the weights

    for (auto i : partialIn1) //generating partial products
    {
        for (auto j : partialIn2)
        {
            comp = new AND({i, j});
            comp->SetOutputs();
            compList.push_back(comp);
            generatedPartials.push_back(comp->returnOutputs()[0]); //AND has just one output
        }
    }

    PartialProduct::LevelizePartials(LevelizedPartials, generatedPartials); //adding the generated partial products to the levelized hash

    map<int, int> r; //to final result to pass to the PPA

    for (auto i : LevelizedPartials)
    {
        r[i.first] = i.second.size();
    }

    GenerateHeader(nIn1, nIn2, LevelizedPartials.size(), "U_SP", file);
    GenereateInOutSig(nIn1, nIn2, r, file);
    vector<int> signalIDs = Component::collectIDs(compList);
    map<int, string> wireHash = generateWires(nIn1, nIn2, signalIDs, LevelizedPartials, file);
    GenerateComponents(wireHash, compList, file);
    file += "endmodule\n";

    return r;
}