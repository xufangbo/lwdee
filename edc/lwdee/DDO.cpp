#include "DDO.h"

void DDO::write(ByteSpan_ref datablock) { DdoManager::set(id, datablock); }
ByteSpan_ref DDO::read() {return DdoManager::get(id); }
void  DDO::release(){DdoManager::remove(id);}

std::atomic<long> DdoManager::incresement;
std::map<long, ByteSpan_ref> DdoManager::ddos = {};
void DdoManager::set(long id, ByteSpan_ref blockdata) { ddos[id] = blockdata; }
ByteSpan_ref DdoManager::get(long id) { return ddos[id]; }
void DdoManager::remove(long id) { ddos.erase(id); }