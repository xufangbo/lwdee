#include "MapDCO.h"
#include <iostream>

std::string  MapDCO::f1(std::string a) {
    std::cout << "call f1(" << a << ")"<< std::endl; 
    return a;
}
std::string  MapDCO::f2(std::string a) {
    std::cout << "call f2(" << a << ")" << std::endl; 
    return a;
}
MapDCO::MapDCO()
{
    getFunctionTable()["f1"] = (PTR)&MapDCO::f1;
    getFunctionTable()["f2"] = (PTR)&MapDCO::f2;
}
MapDCO::~MapDCO()
{

}


