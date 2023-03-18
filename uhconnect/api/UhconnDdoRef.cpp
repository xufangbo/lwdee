#include "UhconnDdoRef.h"
#include "core/UhconnSimpleAddr.h"

UhconnDdoRef::UhconnDdoRef()
{
    id = 0;
    voxorId = "";
}

UhconnDdoRef::~UhconnDdoRef()
{

}

int UhconnDdoRef::itsWorkNodeId(void) {
    return UhconnSimpleAddr(itsVoxorId()).workNodeSn();
}