#include "DefaultProtocal.hpp"

#include "DefaultProtocalHeader.hpp"
#include "DefaultStream.hpp"
#include "TcpResponse.hpp"
#include "core/Stopwatch.hpp"
#include "net/IRunway.hpp"
#include "net/log.hpp"

void DefaultProtocal::server_accept(IRunway* runway, Connection* connection, BufferStream* inputStream) {
  DefaultProtocalHeader header = DefaultProtocalHeader::parse(inputStream);

  auto fun = TcpResponse::find(header.path);
  if (fun == nullptr) {
    logger_error("can't hint path: %s", header.path.c_str());
    return;
  }

  auto outputStream = new DefaultStream(); // send task 中释放

  uint32_t rec1 = Stopwatch::currentMilliSeconds() - header.sen1;
  header.setHeader(outputStream, header.path, header.sen1, rec1, 0, 0);

  (*fun)(inputStream, outputStream);
  delete inputStream;
  inputStream = nullptr;

  uint32_t sen2 = Stopwatch::currentMilliSeconds() - header.sen1;
  header.setsen2(outputStream, sen2);
  header.setLength(outputStream);

  // sendQueue->push(socket, outputStream);
  runway->addSendTask(connection, outputStream);
  runway->qps()->time(header.rec1_sen1());

  // leopard_debug("> response %s", header.path.c_str());
}