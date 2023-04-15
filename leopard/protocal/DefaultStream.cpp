#include "DefaultStream.hpp"

// DefaultStream::DefaultStream() { buffer = (byte*)calloc(1, capacity); }

// DefaultStream::~DefaultStream() {
//   if (buffer != NULL) {
//     free(buffer);
//     buffer = NULL;
//   }
// }


bool DefaultStream::isEnd() {
  if (pos < sizeof(uint64_t)) {
    return false;
  }

  uint64_t len;
  this->get<uint64_t>(0, len);

  return _size >= len;
}