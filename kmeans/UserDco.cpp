#include "UserDco.h"
#include <iostream>

std::string  UserDco::f1(std::string a) {
    std::cout << "call f1(" << a << ")"<< std::endl; 
    return a;
}
std::string  UserDco::f2(std::string a) {
    std::cout << "call f2(" << a << ")" << std::endl; 
    return a;
}
UserDco::UserDco()
{
    getFunctionTable()["f1"] = (PTR)&UserDco::f1;
    getFunctionTable()["f2"] = (PTR)&UserDco::f2;
}
UserDco::~UserDco()
{

}


