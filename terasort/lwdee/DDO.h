#pragma once

#include <atomic>
#include <map>
#include <memory>

#include "api/UhconnDdo.h"
#include "api/UhconnDdoRef.h"
#include "core/ByteSpan.h"

typedef UhconnDdoRef DDOId;

class DDO {
 public:
  DDOId ddoId;

 public:
  DDO() {}
  DDO(DDOId id);

 public:
  void write(ByteSpan_ref datablock);
  ByteSpan_ref read();
  void release();
};