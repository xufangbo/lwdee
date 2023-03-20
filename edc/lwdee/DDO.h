#pragma once

#include <atomic>
#include <map>
#include <memory>

#include "api/UhconnDdo.h"
#include "api/UhconnDdoRef.h"
#include "core/ByteSpan.h"

typedef UhconnDdoRef DDOId;
typedef std::shared_ptr<UhconnDdo> UhconnDdo_ref;

class DDO {
 public:
  DDOId ddoId;
  UhconnDdo_ref uh_ddo;

 public:
  DDO() {}
  DDO(DDOId id);
  DDO(DDOId id,UhconnDdo *uh_ddo);

 public:
  void write(ByteSpan_ref datablock);
  ByteSpan_ref read();
  void release();
};