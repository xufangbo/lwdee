#pragma once

#include "BufferStream.hpp"

class LeopardProtocal {
 public:
  virtual void setHeader(BufferStream *outputStream, std::string &path) = 0;
  virtual void setLength(BufferStream *outputStream) = 0;
};

typedef std::shared_ptr<LeopardProtocal> LeopardProtocalPtr;