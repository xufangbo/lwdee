#pragma once

#include <iostream>
#include "DefaultStream.hpp"
#include "net/LeopardProtocal.hpp"

class DefaultProtocalHeader : public ProtocalHeader {
 public:
};

class DefaultProtocal : public LeopardProtocal {
 public:
  void setHeader(BufferStream* outputStream, std::string& path, uint64_t sen1,uint32_t rec1, uint32_t sen2, uint32_t rec2) override;
  void setLength(BufferStream* outputStream) override;
  void setsen2(BufferStream* outputStream,uint32_t sen2) override;
  ProtocalHeaderPtr getHeader(BufferStream* inputStream) override;
};