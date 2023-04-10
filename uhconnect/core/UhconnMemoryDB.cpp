#include "UhconnMemoryDB.h"
#include <cstdint>
UhconnMemoryDB::UhconnMemoryDB()
{

}

UhconnMemoryDB::~UhconnMemoryDB()
{

}

UhconnMemoryDB::UhconnMemoryDB(int nodeid) {
    _nodeid = nodeid;
}

bool UhconnMemoryDB::ifExist(uint64_t did_) {
    return _dict.find(did_) != _dict.end();
}
