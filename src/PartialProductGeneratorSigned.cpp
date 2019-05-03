#include "PartialProductGeneratorSigned.hpp"

map<int, int> PartialProductGeneratorSigned(int nIn1, int nIn2, string &file) // Get two integer numbers as input sizes and return the levelized generated partial products
{
    assert(nIn1 > 0 && nIn2 > 0 && "Lenght of input bits should be bigger than zero");

    int nmax = (nIn1>nIn2)?nIn1:nIn2;

    vector<PartialProduct> partialIn1 = PartialProduct::createPartialArray(nIn1); //create an array of partial products for the first input
    vector<PartialProduct> partialIn2 = PartialProduct::createPartialArray(nIn2); //create an array of partial products for the second input

    //we should extend the sign bit for the smallest input
    if (nmax==nIn1)
        PartialProduct::ExtendSignBit(partialIn2, nmax);
    else
        PartialProduct::ExtendSignBit(partialIn1, nmax);
    //

    vector<PartialProduct> generatedPartials;

    Component *comp;              //a temp component to store a just created component
    vector<Component *> compList; //the array of used components during implementation

    map<int, vector<PartialProduct>> LevelizedPartials; //for hashing partial products based on the weights

    for (auto i = 0u; i < partialIn1.size(); i++)
    {
        for (auto j = 0u; j < partialIn2.size(); j++)
        {
            if ((i == partialIn1.size() - 1 || j == partialIn2.size() - 1) && !(i == partialIn1.size() - 1 && j == partialIn2.size() - 1))
                comp = new NAND({partialIn1[i], partialIn2[j]});
            else
                comp = new AND({partialIn1[i], partialIn2[j]});
            comp->SetOutputs();
            compList.push_back(comp);
            generatedPartials.push_back(comp->returnOutputs()[0]); //AND and NAND have just one output
        }
    }

    PartialProduct one1(nmax);
    PartialProduct one2(nmax*2-1);

    comp = new ConstantOne({one1});
    comp->SetOutputs();
    compList.push_back(comp);
    generatedPartials.push_back(comp->returnOutputs()[0]);

    comp = new ConstantOne({one2});
    comp->SetOutputs();
    compList.push_back(comp);
    generatedPartials.push_back(comp->returnOutputs()[0]);


    PartialProduct::LevelizePartials(LevelizedPartials, generatedPartials); //adding the generated partial products to the levelized hash

    map<int, int> r; //to final result to pass to the PPA

    for (auto i : LevelizedPartials)
    {
        r[i.first] = i.second.size();
    }

    GenerateHeader(nIn1, nIn2, LevelizedPartials.size(), "S_SP", file);
    GenereateInOutSig(nIn1, nIn2, r, file);
    vector<int> signalIDs = Component::collectIDs(compList);
    map<int, string> wireHash = generateWires(nIn1, nIn2, signalIDs, LevelizedPartials, file);
    GenerateComponents(wireHash, compList, file);
    file += "endmodule\n";

    return r;
}
