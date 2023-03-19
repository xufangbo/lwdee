#include "DDO.h"

#include "api/UhconnDdo.h"

void DDO::write(ByteSpan_ref bytesSpan) {
  UhconnDdo uh_ddo(ddoId);

  DdoBlockData blockdata;
  blockdata.len = bytesSpan->size;
  blockdata.data = bytesSpan->buffer;

  uh_ddo.storeBlock(blockdata);
}

ByteSpan_ref DDO::read() {
  UhconnDdo uh_ddo(ddoId);
  DdoBlockData blockdata;
  uh_ddo.loadBlock(blockdata);

  ByteSpan_ref bytes = std::make_shared<ByteSpan>(blockdata.len);
  bytes->puts((Byte *)blockdata.data, bytes->size);

  return bytes;
}

void DDO::release() {
  UhconnDdo uh_ddo(ddoId);
  uh_ddo.deleteBlock();
}