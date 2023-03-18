#pragma once

#include <atomic>
#include <map>
// #include "api/UhconnDdoRef.h"
#include "core/ByteSpan.h"

class DDO {
  // UhconnDdoRef ref;
 public:
  long id;

 public:
  // DDO(const std::string& vid, DdoDataId ddoId = 0) : ref(vid, ddoId) {}
  void write(ByteSpan_ref datablock);
  ByteSpan_ref read();
};

class DdoManager {
 private:
  static std::atomic<long> incresement;
  static std::map<long, ByteSpan_ref> ddos;

 public:
  static long generateId() { return ++incresement; };
  static void set(long id, ByteSpan_ref blockdata);
  static ByteSpan_ref get(long id);
};