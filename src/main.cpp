#include "GenMul.hpp"
#include <fstream>


//TODO: Adding later command line features
//#pragma GCC diagnostic ignored "-Wunused-parameter"
int main(int argc, char **argv)
{
    string firstStageString, secondStageString, thirdStageString;
    int firstStage, secondStage, thirdStage;

    int in1Size, in2Size;
    string fileAddress;

    // test
    //cout<<"argc: "<<argc<<endl;
    bool cmdline = false;
    if (argc==2) {
      if (std::string(argv[1]) == "A")  {
          //moduleConnector(in1Size, in2Size, fileAddress, firstStage, secondStage, thirdStage);
          //moduleConnector(10, 10, "test-out-A.v", 1 , 1, 1);
	  in1Size = 10; in2Size = 10; fileAddress = "test-out-A.v"; firstStage = 1; secondStage = 1; thirdStage = 1;
	  cmdline = true;
	  cout << "Running in cmd line mode "<<endl;
      }
    }

  if (cmdline == false ) {
    cout<<"Compilation time: "<<GLOBAL_COMPILATION_TIME<<endl;
    cout<<"Compilation SHA256 message digest: "<<GLOBAL_SHA256<<endl<<endl;
    cout<<"/-------------------------------------------------------------------------\\"<<endl;
    cout<<"|  Multiplier Generator GenMul                                            |"<<endl;
    cout<<"|                                                                         |"<<endl;
    cout<<"|  Copyright (c) 2019 Group of Computer Architecture.                     |"<<endl;
    cout<<"|                                                                         |"<<endl;
    cout<<"|  You can find GenMul at: http://www.sca-verification.org/genmul         |"<<endl;
    cout<<"|  Contact us at genmul@sca-verification.org                              |"<<endl;
    cout<<"\\-------------------------------------------------------------------------/"<<endl;
    cout<<endl;
    cout<<"Enter the number indicating Partial Product Generator (PPG) architecture: "<<endl;
    cout<<"1. Unsigned PPG"<<endl;
    cout<<"2. Signed PPG"<<endl;
    cout<<">> ";
    cin>>firstStageString;
    cout<<"*************************************************************************"<<endl<<flush;
    if (firstStageString!="1" && firstStageString!="2")
    {
        cout<<"Wrong input!!!"<<endl;
        return 0;
    }
    firstStage = stoi(firstStageString);

    cout<<"Enter the number indicating Partial Product Accumulator (PPA) architecture: "<<endl;
    cout<<"1. Array"<<endl;
    cout<<"2. Wallace tree"<<endl;
    cout<<"3. Dadda tree"<<endl;
    cout<<"4. Counter-based Wallace tree"<<endl;
    cout<<">> ";
    cin>>secondStageString;
    cout<<"*************************************************************************"<<endl;
    if (secondStageString!="1" && secondStageString!="2" && secondStageString!="3" && secondStageString!="4")
    {
        cout<<"Wrong input!!!"<<endl;
        return 0;
    }
    secondStage = stoi(secondStageString);

    cout<<"Enter the number indicating Final Stage Adder (FSA) architecture: "<<endl;
    cout<<"1. Ripple Carry Adder"<<endl;
    cout<<"2. Carry Look-Ahead Adder"<<endl;
    cout<<"3. Lander-Fischer Adder"<<endl;
    cout<<"4. Kogge-Stone Adder"<<endl;
    cout<<"5. Brent-Kung Adder"<<endl;
    cout<<"6. Carry Skip Adder"<<endl;
    cout<<">> ";
    cin>>thirdStageString;
    cout<<"*************************************************************************"<<endl;
    if (thirdStageString!="1" && thirdStageString!="2" && thirdStageString!="3" && thirdStageString!="4" && thirdStageString!="5" && thirdStageString!="6")
    {
        cout<<"Wrong input!!!"<<endl;
        return 0;
    }
    thirdStage = stoi(thirdStageString);

    //int in1Size, in2Size;
    cout<<"First input size: ";
    cin >> in1Size;
    if (cin.fail() || in1Size<=0)
    {
        cout<<"Wrong input!!!"<<endl;
        return 0;
    }

    cout<<"Second input size: ";
    cin >> in2Size;
    if (cin.fail() || in2Size<=0)
    {
        cout<<"Wrong input!!!"<<endl;
        return 0;
    }
    //cout<<"*************************************************************************"<<endl;
    //string fileAddress = "Multiplier.v";
    //cout<<"Output file: ";
    //cin >> fileAddress;

  } // end of cmdline == false

    string name = GenMulNameMaker(in1Size, in2Size, firstStage, secondStage, thirdStage);
    cout<<"*************************************************************************"<<endl;
    cout<<"Output file: "<<name<<endl;

    string finalCode = GenMul(in1Size, in2Size, firstStage, secondStage, thirdStage);

    ofstream file;
    file.open(name);

    file << finalCode;

    //moduleConnector(10, 5, "10bit-SPS-WL-CK.v");
    //ofstream file("48bit.v");
    //CarrySkipAdderVariable(48, 48, file);
}
