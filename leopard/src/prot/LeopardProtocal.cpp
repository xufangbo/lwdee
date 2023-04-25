#include "LeopardProtocal.hpp"

#include "LeopardHeader.hpp"
#include "LeopardStream.hpp"
#include "TcpRequest.hpp"
#include "TcpResponse.hpp"
#include "core/Stopwatch.hpp"
#include "src/ClientConnection.hpp"
#include "src/log.hpp"

BufferStream* LeopardProtocal::newStream(std::string& path) {
  BufferStream* outputStream = new LeopardStream();  // send task 中释放
  LeopardHeader::create(outputStream, path);

  return outputStream;
}

uint64_t LeopardProtocal::csend(ClientConnection* connection, BufferStream* outputStream) {
  LeopardHeader::setLength(outputStream);
  auto msgId = LeopardHeader::getMessageId(outputStream);

  connection->push(outputStream);
  connection->send(SendSource::request);  // ET 才主动触发

  return msgId;
}

void LeopardProtocal::caccept(Connection* connection, BufferStream* inputStream) {
  LeopardHeader header = LeopardHeader::parse(inputStream);

  header.rec2 = Stopwatch::currentMilliSeconds() - header.sen1;

  // leopard_trace(header.to_string().c_str());

  ClientConnection* socket = (ClientConnection*)(connection->socket);
  auto waiter = socket->findWaiter(header.messageId);
  if (waiter == nullptr || waiter.use_count() == 0) {
    logger_error("waiter is null");  // 什么情况下为null?
  }

  auto callback = TcpRequest::find(header.path);
  if (callback != nullptr) {
    (*callback)(inputStream);
    delete inputStream;
    inputStream = nullptr;

    if (waiter != nullptr) {
      waiter->notify(WaitStatus::succeed);
    }

  } else {
    connection->close(CloseType::no_router);
    if (waiter != nullptr) {
      waiter->notify(WaitStatus::nohint);
    }
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

void LeopardProtocal::saccept(Connection* connection, BufferStream* inputStream) {
  // printf("%s\n", LeopardHeader::to_hex(inputStream).c_str());
  LeopardHeader header = LeopardHeader::parse(inputStream);

  auto fun = TcpResponse::find(header.path);
  if (fun == nullptr) {
    logger_error("can't hint path: %s", header.path.c_str());
    connection->close(CloseType::no_router);
    return;
  }

  auto outputStream = new LeopardStream();  // send task 中释放

  uint32_t rec1 = Stopwatch::currentMilliSeconds() - header.sen1;
  header.copyRequest(outputStream, header);

  (*fun)(inputStream, outputStream);
  delete inputStream;
  inputStream = nullptr;

  header.setsen2(outputStream);
  header.setLength(outputStream);

  connection->push(outputStream);
  connection->send(SendSource::request);  // ET 才主动触发

  // leopard_debug("> response %s", header.path.c_str());
}