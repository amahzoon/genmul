#ifndef PP
#define PP

#include <vector>
#include <map>
using namespace std;

class PartialProduct
{ //partial product class (primary inputs in Mults are partial products too)
public:
  PartialProduct(int w); //w: weight
  int returnWeight();    //return the weight of the partial product
  int returnNo();        //return the identification number of the partial product
  void printInfo();
  static PartialProduct addPartialProducts(PartialProduct a, PartialProduct b);                                        //adding two partial products
  static vector<PartialProduct> createPartialArray(int arrayNumber);                                                   //creating an array of partial products as an integer number
  static void LevelizePartials(map<int, vector<PartialProduct>> &lPartial, vector<PartialProduct> vPartial); //levelize partial products based on the weights
  static vector<PartialProduct> reducePartials(vector<PartialProduct> &vPartial);                                      //reduce partial products and generate new partials
  static int count;                                                                                                    //to keep the number of generated partial products
  static void ExtendSignBit(vector<PartialProduct> &vPartial, int max);
  static void SetCountZero();
  void ChangeWeight(int newWeight);

private:
  int weight; //specify the weight of the partial product
  int no;     //a number to identify the partial product
};

#endif

