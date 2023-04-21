#include "LeopardProtocal.hpp"

#include "LeopardHeader.hpp"
#include "LeopardStream.hpp"
#include "TcpRequest.hpp"
#include "TcpResponse.hpp"
#include "core/Stopwatch.hpp"
#include "net/ClientSocket.hpp"
#include "net/log.hpp"

BufferStream* LeopardProtocal::newStream(std::string& path) {
  BufferStream* outputStream = new LeopardStream();  // send task 中释放
  LeopardHeader().setHeader(outputStream, path, Stopwatch::currentMilliSeconds(), 0, 0, 0);

  return outputStream;
}

void LeopardProtocal::send(IRunway* runway, Connection* connection, BufferStream* outputStream) {
  LeopardHeader().setLength(outputStream);
  runway->addSendTask(connection, outputStream);
}

void LeopardProtocal::caccept(IRunway* runway, Connection* connection, BufferStream* inputStream) {
  LeopardHeader header = LeopardHeader::parse(inputStream);

  header.rec2 = Stopwatch::currentMilliSeconds() - header.sen1;

  // leopard_trace(header.to_string().c_str());

  ClientSocket* socket = (ClientSocket*)(connection->socket);
  auto waiter = socket->popWaiter();
  if (waiter == nullptr || waiter.use_count() == 0) {
    logger_error("waiter is null");
  } else {
    leopard_trace("get waiter [%d]", waiter->getId());
  }

  auto callback = TcpRequest::find(header.path);
  if (callback != nullptr) {
    (*callback)(inputStream);
    delete inputStream;
    inputStream = nullptr;

    waiter->notify(WaitStatus::succeed);
  } else {
    waiter->notify(WaitStatus::nohint);
    logger_error("can't hint path %s", header.path.c_str());
  }

#ifdef LEOPARD_SUSPEND
  auto suspend = TcpRequest::findSuspend(path);
  if (suspend != nullptr) {
    suspend->callback(inputStream);
    return;
  }
#endif
}

void LeopardProtocal::saccept(IRunway* runway, Connection* connection, BufferStream* inputStream) {
  LeopardHeader header = LeopardHeader::parse(inputStream);

  auto fun = TcpResponse::find(header.path);
  if (fun == nullptr) {
    logger_error("can't hint path: %s", header.path.c_str());
    return;
  }

  auto outputStream = new LeopardStream();  // send task 中释放

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