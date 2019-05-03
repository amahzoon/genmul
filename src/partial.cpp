#include "partial.hpp"
#include <iostream>


PartialProduct::PartialProduct(int w)
{
    this->no = this->count;
    this->weight = w;
    this->count++;
}
///////////////////////////////////////////////////
PartialProduct PartialProduct::addPartialProducts(PartialProduct a, PartialProduct b)
{
    int w1 = a.weight;
    int w2 = b.weight;
    PartialProduct out(w1 + w2);
    return out;
}
///////////////////////////////////////////////////
void PartialProduct::printInfo()
{
    cout << "Number: " << this->no << endl;
    cout << "Weight: " << this->weight << endl;
}
///////////////////////////////////////////////////
vector<PartialProduct> PartialProduct::createPartialArray(int arrayNumber)
{
    vector<PartialProduct> out;
    for (int i = 0; i < arrayNumber; i++)
    {
        PartialProduct a(i);
        out.push_back(a);
    }
    return out;
}
///////////////////////////////////////////////////
int PartialProduct::returnWeight()
{
    return this->weight;
}
///////////////////////////////////////////////////
int PartialProduct::returnNo()
{
    return this->no;
}
//////////////////////////////////////////////////
void PartialProduct::LevelizePartials(map<int, vector<PartialProduct>> &lPartial , vector<PartialProduct> vPartial)
{
    for (auto i: vPartial)
    {
        lPartial[i.weight].push_back(i);
    }
}   
/////////////////////////////////////////////////
void PartialProduct::SetCountZero()
{
    count = 0;
}
/////////////////////////////////////////////////
void PartialProduct::ExtendSignBit(vector<PartialProduct> &vPartial, int max)
{
    for (int i=vPartial.size(); i<max; i++)
    {
        vPartial.push_back(vPartial[i-1]);
        vPartial[i].ChangeWeight(i);
    }
}
/////////////////////////////////////////////////
void PartialProduct::ChangeWeight(int newWeight)
{
    this->weight = newWeight;
}
