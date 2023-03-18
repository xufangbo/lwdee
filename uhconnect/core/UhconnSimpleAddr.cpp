#include "UhconnSimpleAddr.h"
#include <sstream>

using namespace std;
UhconnSimpleAddr::UhconnSimpleAddr(int worknodeSn, int voxorSn)
{
    stringstream ss;
    ss << worknodeSn <<" "<< voxorSn;
    value = ss.str();
  
}

UhconnSimpleAddr::~UhconnSimpleAddr()
{

}

int UhconnSimpleAddr::workNodeSn(void){
    int vSn, nSn;
    stringstream ss(value);
    ss >> nSn >> vSn;
    return nSn;

}
int UhconnSimpleAddr::voxorSn(void){
    int vSn, nSn;
    stringstream ss(value);
    ss >> nSn >> vSn;
    return vSn;
}
