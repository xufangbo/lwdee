#ifndef UHCONNMEMORYDATACELL_H
#define UHCONNMEMORYDATACELL_H

#pragma once

#include <boost/atomic.hpp>

class UhconnMemoryDataCell
{
public:
    UhconnMemoryDataCell(int refcnt_);
private:
    UhconnMemoryDataCell();
    ~UhconnMemoryDataCell();

    boost::atomic_int _rcnt;
};

#endif