#pragma once

#include "net/BufferStream.hpp"
// #include <memory.h>
// #include <iomanip>
// #include <iostream>
// #include <memory>
// #include <sstream>

#define BUF_UNIT 1024

typedef void byte;

class DefaultStream : public BufferStream {
 public:
  bool isEnd() override;
  size_t currentSize() override;

 public:
  DefaultStream()
      : BufferStream() {}
  DefaultStream(size_t len)
      : BufferStream(len) {}

 private:
  std::shared_ptr<BufferStream> newInstance(size_t len);
};