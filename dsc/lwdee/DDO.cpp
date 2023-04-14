#include "DDO.h"

#include "core/Exception.hpp"
#include "core/UhconnSimpleDB.h"
#include "core/log.hpp"

DDO::DDO(DDOId id)
    : ddoId(id) {}

void DDO::write(ByteSpan_ref bytesSpan) {
  DdoBlockData blockdata;
  blockdata.type = 22;  // ?
  blockdata.len = bytesSpan->size;
  blockdata.data = bytesSpan->buffer;

  bytesSpan->releaseOwner();

  UhconnSimpleDB::getInstance().storeBlock(ddoId.itsId(), std::move(blockdata));
}

void DDO::write(std::string& str) {
  DdoBlockData blockdata;

  blockdata.len = str.size();
  blockdata.data = new char[str.size()];

  memcpy(blockdata.data, str.data(), str.size());

  UhconnSimpleDB::getInstance().storeBlock(ddoId.itsId(), std::move(blockdata));
}

string_ref DDO::read() {
  DdoBlockData* blockdata = UhconnSimpleDB::getInstance().getBlock(ddoId.itsId(), ddoId.itsWorkNodeId());
  if (blockdata == nullptr) {
    auto message = std::string("failed read ddo(node") + std::to_string(ddoId.itsWorkNodeId()) + ")";
    throw LwdeeException(message, ZONE);
  }

  string_ref output = std::make_shared<std::string>(blockdata->len, '\0');
  memcpy((void*)output->data(), blockdata->data, blockdata->len);

  return output;
}

void DDO::release() {
  // uh_ddo->deleteBlock();
  // uh_ddo.reset();

  UhconnSimpleDB::getInstance().deleteBlock(ddoId.itsId());
}

void DDO::releaseGlobal() {
  UhconnSimpleDB::getInstance().deleteBlockGlobal(ddoId.itsId());
}