#pragma once

#include "DefaultStream.hpp"
#include "net/LeopardProtocal.hpp"

class DefaultProtocal : public LeopardProtocal {
 public:
  void setHeader(BufferStream *outputStream, std::string &path) override;
  void setLength(BufferStream *outputStream) override;
};