#pragma once

#include "BufferStream.hpp"
#include "ProtocalHeader.hpp"

class LeopardProtocal {
 public:
  virtual void setHeader(BufferStream* outputStream, std::string& path, uint64_t sen1, uint32_t rec1, uint32_t sen2, uint32_t rec2) = 0;
  virtual void setLength(BufferStream* outputStream) = 0;
  virtual void setsen2(BufferStream* outputStream, uint32_t sen2) = 0;
  virtual void parseHeader(BufferStream* inputStream, ProtocalHeader* header) = 0;
  virtual ProtocalHeaderPtr newHeader() = 0;
};

typedef std::shared_ptr<LeopardProtocal> LeopardProtocalPtr;