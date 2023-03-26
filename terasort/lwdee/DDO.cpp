#include "DDO.h"

#include "core/Exception.hpp"
#include "core/UhconnSimpleDB.h"
#include "core/log.hpp"

DDO::DDO(DDOId id) : ddoId(id) {}

void DDO::write(ByteSpan_ref bytesSpan) {
  DdoBlockData blockdata;
  blockdata.type = 22;  // ?
  blockdata.len = bytesSpan->size;
  blockdata.data = bytesSpan->buffer;

  bytesSpan->releaseOwner();

  UhconnSimpleDB::getInstance().storeBlock(ddoId.itsId(), blockdata);
}

void DDO::write(std::string &str) {
  DdoBlockData blockdata;
  
  blockdata.len = str.size();
  blockdata.data = new char[str.size()];

  memcpy(blockdata.data, str.data(), str.size());

  UhconnSimpleDB::getInstance().storeBlock(ddoId.itsId(), blockdata);
}

ByteSpan_ref DDO::read() {
  // DdoBlockData blockdata;
  // UhconnSimpleDB::getInstance().loadBlock(ddoId.itsId(), blockdata);
  DdoBlockData *blockdata = UhconnSimpleDB::getInstance().getBlock(
      ddoId.itsId(), ddoId.itsWorkNodeId());
  if (blockdata == nullptr) {
    throw LwdeeException("failed read ddo", ZONE);
  }

  ByteSpan_ref bytes = std::make_shared<ByteSpan>(blockdata->len);
  bytes->puts((Byte *)blockdata->data, bytes->size);
  bytes->reset();

  // bytes->buffer = (Byte*)blockdata.data;
  // bytes->size = blockdata.len;

  return bytes;
}

void DDO::release() {
  // uh_ddo->deleteBlock();
  // uh_ddo.reset();

  UhconnSimpleDB::getInstance().deleteBlock(ddoId.itsId());
}

void DDO::releaseGlobal() {
  UhconnSimpleDB::getInstance().deleteBlockGlobal(ddoId.itsId());
}