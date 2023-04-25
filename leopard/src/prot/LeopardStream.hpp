#pragma once

#include "src/BufferStream.hpp"
// #include <memory.h>
// #include <iomanip>
// #include <iostream>
// #include <memory>
// #include <sstream>

#define BUF_UNIT 1024

typedef void byte;

class LeopardStream : public BufferStream {
 public:
  bool isEnd() override;
  size_t currentSize() override;

 public:
  LeopardStream()
      : BufferStream() {}
  LeopardStream(size_t len)
      : BufferStream(len) {}

 private:
  BufferStream* newInstance(size_t len);
};