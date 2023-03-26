#include "MapDCO.h"
#include <iostream>

std::vector<DDO> MapDCO::ddos;

std::string  MapDCO::map(std::string a) {
    std::cout << "call f1(" << a << ")"<< std::endl; 
    return a;
}
std::string  MapDCO::ddo(std::string voxorId,DdoDataId ddoId) {
    std::cout << "call f2(" << voxorId << ")" << std::endl; 
    return voxorId;
}
MapDCO::MapDCO()
{
    getFunctionTable()["map"] = (PTR)&MapDCO::map;
    getFunctionTable()["ddo"] = (PTR)&MapDCO::ddo;
}
MapDCO::~MapDCO()
{

}


