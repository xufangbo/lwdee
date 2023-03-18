#ifndef UHCONNMEMORYDB_H
#define UHCONNMEMORYDB_H

#pragma once

#include <map>
#include <iostream>

class UhconnMemoryDataCell;
class UhconnMemoryDB
{
public:
    UhconnMemoryDB(int nodeid);
    ~UhconnMemoryDB();

    UhconnMemoryDataCell* makeDataCell(uint64_t did_, void* data_, uint64_t dlen_);

    void deleteDataCell(UhconnMemoryDataCell*);

    bool ifExist(uint64_t did_);

    typedef std::map<uint64_t, UhconnMemoryDataCell*> dict_t; 

private:
    UhconnMemoryDB();
    int _nodeid;
    const dict_t _dict;
};

#endif