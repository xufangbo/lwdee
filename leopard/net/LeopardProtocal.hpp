#pragma once

#include "BufferStream.hpp"

class ProtocalHeader {
 public:
  uint64_t totalLength;
  uint64_t time;
  uint32_t elapsed;
  uint32_t pathLength;
  std::string path;

  int headerSize() {
    return sizeof(totalLength) + sizeof(time) + sizeof(pathLength) + path.size();
  }
};

typedef std::shared_ptr<ProtocalHeader> ProtocalHeaderPtr;

class LeopardProtocal {
 public:
  virtual void setHeader(BufferStream* outputStream, std::string& path) = 0;
  virtual void setLength(BufferStream* outputStream) = 0;
  virtual ProtocalHeaderPtr getHeader(BufferStream* inputStream) = 0;
};

typedef std::shared_ptr<LeopardProtocal> LeopardProtocalPtr;