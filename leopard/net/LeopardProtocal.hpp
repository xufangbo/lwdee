#pragma once

#include "BufferStream.hpp"

class ProtocalHeader {
 public:
  uint64_t totalLength;
  uint64_t rx1;
  uint32_t tx1;
  uint32_t rx2;
  uint32_t tx2;
  uint32_t pathLength;
  std::string path;

  int headerSize() {
    return sizeof(totalLength) + sizeof(rx1) + sizeof(tx1) + sizeof(rx2) + sizeof(tx2) + sizeof(pathLength) + path.size();
  }

 public:
  float tx1_rx1() { return tx1 * 1.0 / 1000; }
  float tx2_rx2() { return (tx2 - rx2) * 1.0 / 1000; }
  float tx2_rx1() { return tx2 * 1.0 / 1000; }
};

typedef std::shared_ptr<ProtocalHeader> ProtocalHeaderPtr;

class LeopardProtocal {
 public:
  virtual void setHeader(BufferStream* outputStream, std::string& path) = 0;
  virtual void setLength(BufferStream* outputStream) = 0;
  virtual ProtocalHeaderPtr getHeader(BufferStream* inputStream) = 0;
};

typedef std::shared_ptr<LeopardProtocal> LeopardProtocalPtr;