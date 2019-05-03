#include "LanderFischerAdder.hpp"

int LanderFischerAdder(int nIn1, int nIn2, string &file) // Get two integer numbers as input sizes and create the carry look ahead adder
{
    assert(nIn1 > 0 && nIn2 > 0 && "Lenght of input bits should be bigger than zero");

    vector<PartialProduct> partialIn1 = PartialProduct::createPartialArray(nIn1); //create an array of partial products for the first input
    vector<PartialProduct> partialIn2 = PartialProduct::createPartialArray(nIn2); //create an array of partial products for the second input
    vector<PartialProduct> partialOut;                                            //the array of partial products for the output
    vector<Component *> compList;                                                 //the array of used components during implementation

    map<int, vector<PartialProduct>> LevelizedPartials; //for hashing partial products based on the weights

    PartialProduct::LevelizePartials(LevelizedPartials, partialIn1); //adding the first input to the levelized hash
    PartialProduct::LevelizePartials(LevelizedPartials, partialIn2); //adding the second input to the levelized hash

    vector<PartialProduct> P; //propagate signals
    vector<PartialProduct> G; //Generate signals

    int curentWeight = 0;
    bool endFlag = false; // a flag which indicates the end of the implementation

    Component *comp; //a temp component to store a just created component

    while (!endFlag) //extract P and G by Half-adders
    {
        if (LevelizedPartials[curentWeight].size() == 2) //if there are two partial products with the same weights
        {
            comp = new HalfAdder(LevelizedPartials[curentWeight]);
            LevelizedPartials[curentWeight].clear();
            comp->SetOutputs();
            P.push_back(comp->returnOutputs()[0]); //adding Xi^Yi to the list of propagate signals
            G.push_back(comp->returnOutputs()[1]); //adding Xi&Yi to the list of Generate signals
            compList.push_back(comp);
        }
        else                                                 //if there is only one partial product with the currentWeight
            P.push_back(LevelizedPartials[curentWeight][0]); //adding the single signal to the list of propagate signals
        curentWeight++;
        if (LevelizedPartials[curentWeight].size() == 0)
            endFlag = true;
    }

    vector<PartialProduct> carries;   //to keep the list of all carries
    //PartialProduct firstCarry = G[0]; //G0 is the first carry

    vector<vector<PartialProduct>> grid; //for storing prefix grid

    for (auto i = 0u; i < P.size(); i++)
    {
        if (i < G.size())
            grid.push_back({P[i], G[i]});
        else
            grid.push_back({P[i]});
    }

    int nNeededLevels = ceil(log2(P.size())); //the number of required levels for Lander-Fischer adders

    int gap;                        //gap in the prefix grid
    vector<PartialProduct> tempIn1; //for two vector concatination
    vector<PartialProduct> tempIn2; //for two vector concatination
    int itr;
    /////////////////////////////////////////////
    for (auto i = 0; i < nNeededLevels; i++) //Lander-Fischer algorithm
    {
        gap = pow(2, i);
        for (auto j = gap - 1; j < (int) grid.size(); j += gap * 2)
        {
            itr = j;
            for (int k = 0; k < gap; k++)
            {
                itr++;
                if (itr < (int)grid.size())
                {
                    tempIn1 = grid[j];
                    tempIn2 = grid[itr];
                    tempIn1.insert(tempIn1.end(), tempIn2.begin(), tempIn2.end()); //concatinating two vector

                    comp = new CarryOperator(tempIn1);
                    comp->SetOutputs();
                    compList.push_back(comp);
                    grid[itr] = comp->returnOutputs(); //updating grid values*/
                }
            }
        }
    }
    ///////////////////////////////////////////
    for (auto i: grid)
    {
        if (i.size()==2)
             carries.push_back(i[1]);
    }



    map<int, vector<PartialProduct>> FinalPartials; //we need to xor the Propagtes and carries to obtain the final output
    PartialProduct::LevelizePartials(FinalPartials, P);
    PartialProduct::LevelizePartials(FinalPartials, carries);

    endFlag = false;
    curentWeight = 0;
    while (!endFlag)
    {
        if (FinalPartials[curentWeight].size() == 2) //if there are two partial products with the same weights
        {
            comp = new XOR(FinalPartials[curentWeight]);
            FinalPartials[curentWeight].clear();
            comp->SetOutputs();
            compList.push_back(comp); //adding XORs to the list of components
            PartialProduct::LevelizePartials(FinalPartials, comp->returnOutputs());
        }
        curentWeight++;
        if (FinalPartials[curentWeight].size() < 2)
            endFlag = true;
    }
    for (auto i = 0u; i < FinalPartials.size(); i++) //creating the list of outputs
    {
         if (!FinalPartials[i].empty())
            partialOut.push_back(FinalPartials[i][0]);
    }


    //Generating the output verilog file
    GenerateHeader(nIn1,nIn2, "LF", file);
    GenereateInOutSig (nIn1, nIn2, partialOut.size(), file);

    vector<int> signalComponentIDs = Component::collectIDs(compList); //extracting IDs from the signals of components
    vector<int> signalIDs = signalComponentIDs;
    sort( signalIDs.begin(), signalIDs.end() ); // removing duplicated signals
    signalIDs.erase( unique( signalIDs.begin(), signalIDs.end() ), signalIDs.end() );

    map<int, string> wireHash = generateWires (nIn1, nIn2, signalIDs, partialOut, file);
    GenerateComponents (wireHash, compList, file);
    file+="endmodule\n";
    return partialOut.size();
}
