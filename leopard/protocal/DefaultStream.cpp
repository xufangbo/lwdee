#include "DefaultStream.hpp"

size_t DefaultStream::currentSize() {
  uint64_t len;
  this->get<uint64_t>(0, len);
  return len;
}

bool DefaultStream::isEnd() {
  if (pos < sizeof(uint64_t)) {
    return false;
  }

  uint64_t len;
  this->get<uint64_t>(0, len);

  return _size >= len;
}

std::shared_ptr<BufferStream> DefaultStream::newInstance(size_t len) {
  return std::make_shared<DefaultStream>(len);
}