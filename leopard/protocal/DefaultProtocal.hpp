#pragma once

#include <iostream>
#include "DefaultStream.hpp"
#include "net/LeopardProtocal.hpp"

class DefaultProtocalHeader : public ProtocalHeader {
 public:
 
};

class DefaultProtocal : public LeopardProtocal {
 public:
  void setHeader(BufferStream* outputStream, std::string& path) override;
  void setLength(BufferStream* outputStream) override;
  ProtocalHeaderPtr getHeader(BufferStream *inputStream) override;
};