#include "RippleCarryAdder.hpp"
#include "CarryLookAhead.hpp"
#include "LanderFischerAdder.hpp"
#include "BrentKungAdder.hpp"
#include "KoggeStoneAdder.hpp"
#include "CarrySkipAdder.hpp"
#include "CarrySkipAdderVariable.hpp"
#include "PartialProductGenerator.hpp"
#include "PartialProductGeneratorSigned.hpp"
#include "Array.hpp"
#include "Wallace.hpp"
#include "Wallace5.hpp"
#include "Dadda.hpp"
#include "version.hppgen"


string moduleConnector(int nIn1, int nIn2, int firstStage, int secondStage, int thirdStage); // Connect three stages to create a multiplier
string nameMaker (int nIn1, int nIn2, int firstStage, int secondStage, int thirdStage); //create name for the final Verilog file
