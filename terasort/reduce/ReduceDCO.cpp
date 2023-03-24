#include "ReduceDCO.h"
#include <iostream>

std::string  ReduceDCO::f1(std::string a) {
    std::cout << "call f1(" << a << ")"<< std::endl; 
    return a;
}
std::string  ReduceDCO::f2(std::string a) {
    std::cout << "call f2(" << a << ")" << std::endl; 
    return a;
}
ReduceDCO::ReduceDCO()
{
    getFunctionTable()["f1"] = (PTR)&ReduceDCO::f1;
    getFunctionTable()["f2"] = (PTR)&ReduceDCO::f2;
}
ReduceDCO::~ReduceDCO()
{

}


