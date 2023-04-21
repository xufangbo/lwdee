#include "DefaultProtocal.hpp"

#include "DefaultProtocalHeader.hpp"
#include "DefaultStream.hpp"
#include "TcpRequest.hpp"
#include "core/Stopwatch.hpp"
#include "net/ClientSocket.hpp"
#include "net/log.hpp"

void DefaultProtocal::client_accept(IRunway* runway, Connection* connection, BufferStream* inputStream) {
  DefaultProtocalHeader header = DefaultProtocalHeader::parse(inputStream);

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

BufferStream* DefaultProtocal::client_request(RequestInvoke request, std::string& path) {
  DefaultProtocalHeader header;

  BufferStream* outputStream = new DefaultStream();  // send task 中释放
  header.setHeader(outputStream, path, Stopwatch::currentMilliSeconds(), 0, 0, 0);

  request(outputStream);

  header.setLength(outputStream);

  return outputStream;
}