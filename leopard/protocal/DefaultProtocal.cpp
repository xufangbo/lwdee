#include "DefaultProtocal.hpp"

void DefaultProtocal::setHeader(BufferStream *outputStream, std::string &path) {
  outputStream->put<uint64_t>(0);            // len
  outputStream->put<uint32_t>(path.size());  // path length
  outputStream->put(path);                   // path
}

void DefaultProtocal::setLength(BufferStream *outputStream) {
  outputStream->set<uint64_t>(0, outputStream->size());
}