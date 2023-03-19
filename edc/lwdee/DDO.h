#pragma once

#include <atomic>
#include <map>
#include <memory>

#include "api/UhconnDdoRef.h"
#include "core/ByteSpan.h"

typedef UhconnDdoRef DDOId;

class DDO {

 public:
  DDOId ddoId;

 public:
  DDO() {}
  DDO(DDOId id) : ddoId(id) {}
  void write(ByteSpan_ref datablock);
  ByteSpan_ref read();
  void release();
};

// class DdoManager {
//  private:
//   static std::atomic<DDOId> incresement;
//   static std::map<DDOId, ByteSpan_ref> ddos;

//  public:
//   static DDOId generateId() { return ++incresement; };
//   static void set(DDOId id, ByteSpan_ref blockdata);
//   static ByteSpan_ref get(DDOId id);
//   static void remove(DDOId id);
// };