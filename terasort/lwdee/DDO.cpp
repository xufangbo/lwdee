#include "DDO.h"

#include "core/UhconnSimpleDB.h"
#include "core/log.hpp"
#include "core/Exception.hpp"

DDO::DDO(DDOId id) : ddoId(id) {}

void DDO::write(ByteSpan_ref bytesSpan) {
  DdoBlockData blockdata;
  blockdata.type = 22;  // ?
  blockdata.len = bytesSpan->size;
  blockdata.data = bytesSpan->buffer;

  bytesSpan->releaseOwner();

  UhconnSimpleDB::getInstance().storeBlock(ddoId.itsId(), blockdata);
}

ByteSpan_ref DDO::read() {
  // DdoBlockData blockdata;
  // UhconnSimpleDB::getInstance().loadBlock(ddoId.itsId(), blockdata);
  DdoBlockData *blockdata = UhconnSimpleDB::getInstance().getBlock(ddoId.itsId());
  if(blockdata == nullptr){
    throw LwdeeException("failed read ddo", ZONE);
  }

  ByteSpan_ref bytes = std::make_shared<ByteSpan>(blockdata->len);
  bytes->puts((Byte*)blockdata->data, bytes->size);
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