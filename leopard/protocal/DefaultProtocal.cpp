#include "DefaultProtocal.hpp"

#include "core/Stopwatch.hpp"

void DefaultProtocal::setHeader(BufferStream* outputStream, std::string& path) {
  outputStream->put<uint64_t>(0);            // len
  outputStream->put<uint64_t>(Stopwatch::currentMilliSeconds());  // time
  outputStream->put<uint32_t>(path.size());  // path length
  outputStream->put(path);                   // path
}

void DefaultProtocal::setLength(BufferStream* outputStream) {
  outputStream->set<uint64_t>(0, outputStream->size());
}

ProtocalHeaderPtr DefaultProtocal::getHeader(BufferStream* inputStream) {
  
  auto header = std::make_shared<DefaultProtocalHeader>();

  header->totalLength = inputStream->get<uint64_t>();
  header->time = inputStream->get<uint64_t>();
  header->elapsed = Stopwatch::currentMilliSeconds() - header->time;
  header->pathLength = inputStream->get<uint32_t>();
  header->path = inputStream->getString(header->pathLength);
  
  return header;
}