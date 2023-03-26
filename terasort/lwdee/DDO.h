#pragma once

#include <atomic>
#include <map>
#include <memory>
#include <sstream>

#include "api/UhconnDdo.h"
#include "api/UhconnDdoRef.h"
#include "core/ByteSpan.h"

typedef UhconnDdoRef DDOId;

class DDO {
 public:
  DDOId ddoId;

 public:
  DDO() {}
  DDO(std::string str) {
    std::string vid;
    DdoDataId did;

    std::istringstream ss(str);
    ss >> did >> vid;

    DDOId tmp(vid, did);

    ddoId = tmp;
  }
  DDO(DDOId id);

 public:
  void write(ByteSpan_ref datablock);
  void write(std::string &str);
  ByteSpan_ref read();
  void release();
  void releaseGlobal();
};