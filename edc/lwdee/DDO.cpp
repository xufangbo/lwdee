#include "DDO.h"

void DDO::write(ByteSpan_ref datablock) { DdoManager::set(id, datablock); }
ByteSpan_ref DDO::read() { DdoManager::get(id); }

std::atomic<long> DdoManager::incresement;
std::map<long, ByteSpan_ref> DdoManager::ddos = {};
void DdoManager::set(long id, ByteSpan_ref blockdata) { ddos[id] = blockdata; }
ByteSpan_ref DdoManager::get(long id) { return ddos[id]; }