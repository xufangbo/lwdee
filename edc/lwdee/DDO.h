#pragma once

#include "api/UhconnDdoRef.h"
#include "core/ByteSpan.h"

class DDO {
  UhconnDdoRef ref;

 public:
  DDO(const std::string& vid, DdoDataId ddoId = 0) : ref(vid, ddoId) {}
  void write(ByteSpan* datablock);
  ByteSpan_ref read();
};