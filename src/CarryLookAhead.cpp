#include "CarryLookAhead.hpp"

int CarryLookAhead(int nIn1, int nIn2, string &file) // Get two integer numbers as input sizes and create the carry look ahead adder
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

    vector<CarryPredictor *> predictorList; //the list of all carry predictors
    CarryPredictor *predictor;              //the current carry predictor

    vector<PartialProduct>::const_iterator first, last; //specify the location of the iterator length
    for (auto i = 1u; i <= P.size(); i++)                  //remember that P.size() is always bigger or equal to G.size()
    {
        first = P.begin();
        last = P.begin() + i;
        vector<PartialProduct> PTemp(first, last);

        first = G.begin();
        if (i <= G.size())
            last = G.begin() + i;
        else
            last = G.begin() + G.size();
        vector<PartialProduct> GTemp(first, last);

        predictor = new CarryPredictor(PTemp, GTemp);
        carries.push_back(predictor->returnOutput());
        predictorList.push_back(predictor);
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
    GenerateHeader(nIn1,nIn2, "CL", file);
    GenereateInOutSig (nIn1, nIn2, partialOut.size(), file);

    vector<int> signalComponentIDs = Component::collectIDs(compList); //extracting IDs from the signals of components
    vector<int> signalPredictorIDs = CarryPredictor::collectIDs(predictorList); //extracting IDs from the signals of carry predictors
    vector<int> signalIDs = signalComponentIDs;
    signalIDs.insert(signalIDs.end(), signalPredictorIDs.begin(), signalPredictorIDs.end()); //merging two ID set from components and predictors
    sort( signalIDs.begin(), signalIDs.end() ); // removing duplicated signals
    signalIDs.erase( unique( signalIDs.begin(), signalIDs.end() ), signalIDs.end() );

    map<int, string> wireHash = generateWires (nIn1, nIn2, signalIDs, partialOut, file);
    GenerateComponents (wireHash, compList, file);
    GeneratePredictors (wireHash, predictorList, file);
    file+="endmodule\n";
    return partialOut.size();
}
