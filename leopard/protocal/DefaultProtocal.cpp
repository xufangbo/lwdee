#include "DefaultProtocal.hpp"

#include "core/Stopwatch.hpp"
#include "net/log.hpp"

void DefaultProtocal::setHeader(BufferStream* outputStream, std::string& path, uint64_t sen1, uint32_t rec1, uint32_t sen2, uint32_t rec2) {
  outputStream->put<uint64_t>(0);            // len
  outputStream->put<uint64_t>(sen1);          // sen1
  outputStream->put<uint32_t>(rec1);          // rec1
  outputStream->put<uint32_t>(sen2);          // sen2
  outputStream->put<uint32_t>(rec2);          // rec2
  outputStream->put<uint32_t>(path.size());  // path length
  outputStream->put(path);                   // path
}

void DefaultProtocal::setLength(BufferStream* outputStream) {
  outputStream->set<uint64_t>(0, outputStream->size());
}

void DefaultProtocal::setsen2(BufferStream* outputStream, uint32_t sen2) {
  outputStream->set<uint32_t>(20, sen2);
}

ProtocalHeaderPtr DefaultProtocal::getHeader(BufferStream* inputStream) {
  auto header = std::make_shared<DefaultProtocalHeader>();

  header->totalLength = inputStream->get<uint64_t>();
  header->sen1 = inputStream->get<uint64_t>();
  header->rec1 = inputStream->get<uint32_t>();
  header->sen2 = inputStream->get<uint32_t>();
  header->rec2 = inputStream->get<uint32_t>();
  header->pathLength = inputStream->get<uint32_t>();
  header->path = inputStream->getString(header->pathLength);

  // if (header->rec1 > 1000) {
  //   leopard_info("recive %s , elapsed %.3fs", header->path.c_str(), header->rec1_sen1());
  // } else {
  //   leopard_trace("recive %s , elapsed %.3fs", header->path.c_str(), header->rec1_sen1());
  // }

  return header;
}