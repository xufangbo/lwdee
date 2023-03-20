#include "DDO.h"

#include "api/UhconnDdo.h"
#include "core/log.hpp"

DDO::DDO(DDOId id)
    : ddoId(id) {
  // logger_trace("create ddo :{nodeId:%d,voxorId:%s,ddoId:%ld}", id.itsWorkNodeId(), id.itsVoxorId().c_str(), id.itsId());
}

DDO::DDO(DDOId id, UhconnDdo* uh_ddo)
    : DDO(id) {
  this->uh_ddo.reset(uh_ddo);
}

void DDO::write(ByteSpan_ref bytesSpan) {
  if (uh_ddo == nullptr) {
    uh_ddo.reset(new UhconnDdo(ddoId));
  }

  DdoBlockData blockdata;
  blockdata.len = bytesSpan->size;
  blockdata.data = bytesSpan->buffer;

  uh_ddo->storeBlock(blockdata);
}

ByteSpan_ref DDO::read() {
  DdoBlockData blockdata;
  uh_ddo->loadBlock(blockdata);

  ByteSpan_ref bytes = std::make_shared<ByteSpan>(blockdata.len);
  bytes->puts((Byte*)blockdata.data, bytes->size);
  bytes->reset();

  return bytes;
}

void DDO::release() {
  uh_ddo->deleteBlock();
  uh_ddo.reset();
}