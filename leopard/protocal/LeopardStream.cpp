#include "LeopardStream.hpp"

size_t LeopardStream::currentSize() {
  uint64_t len;
  this->get<uint64_t>(0, len);
  return len;
}

bool LeopardStream::isEnd() {
  if (pos < sizeof(uint64_t)) {
    return false;
  }

  uint64_t len;
  this->get<uint64_t>(0, len);

  return _size >= len;
}

BufferStream* LeopardStream::newInstance(size_t len) {
  return new LeopardStream(len);
}