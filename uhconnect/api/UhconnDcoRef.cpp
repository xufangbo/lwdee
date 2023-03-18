#include "UhconnDcoRef.h"
#include "core/UhconnSimpleAddr.h"

UhconnDcoRef::UhconnDcoRef()
{

}

UhconnDcoRef::~UhconnDcoRef()
{

}

UhconnDcoRef::UhconnDcoRef(const std::string& in_json) {
    voxorId = in_json;

};

int UhconnDcoRef::itsBelongWorkNodeId(void) {
    UhconnSimpleAddr addr(itsBelongVoxorId());
    return addr.workNodeSn();
}