#include "DefaultProtocal.hpp"

#include "core/Stopwatch.hpp"
#include "net/log.hpp"

void DefaultProtocal::setHeader(BufferStream* outputStream, std::string& path) {
  outputStream->put<uint64_t>(0);                                 // len
  outputStream->put<uint64_t>(Stopwatch::currentMilliSeconds());  // rx1
  outputStream->put<uint32_t>(0);                                 // tx1
  outputStream->put<uint32_t>(0);                                 // rx2
  outputStream->put<uint32_t>(0);                                 // tx2
  outputStream->put<uint32_t>(path.size());                       // path length
  outputStream->put(path);                                        // path

  // leopard_trace("(%d)%s", path.size(), path.c_str());
}

void DefaultProtocal::setLength(BufferStream* outputStream) {
  outputStream->set<uint64_t>(0, outputStream->size());
}

ProtocalHeaderPtr DefaultProtocal::getHeader(BufferStream* inputStream) {
  auto header = std::make_shared<DefaultProtocalHeader>();

  header->totalLength = inputStream->get<uint64_t>();
  header->rx1 = inputStream->get<uint64_t>();
  header->tx1 = inputStream->get<uint32_t>();
  header->rx2 = inputStream->get<uint32_t>();
  header->tx2 = inputStream->get<uint32_t>();
  header->pathLength = inputStream->get<uint32_t>();
  header->path = inputStream->getString(header->pathLength);

  header->tx1 = Stopwatch::currentMilliSeconds() - header->rx1;

  if (header->tx1 > 1000) {
    leopard_info("recive %s , elapsed %.3fs", header->path.c_str(), header->tx1_rx1());
  } else {
    leopard_trace("recive %s , elapsed %.3fs", header->path.c_str(), header->tx1_rx1());
  }

  return header;
}