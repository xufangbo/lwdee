#include "SocketClient.hpp"

#include "Antelope.hpp"
#include "core/Exception.hpp"
#include "core/Stopwatch.hpp"
#include "net/LeopardProtocal.hpp"
#include "net/ProtocalFactory.hpp"
#include "net/log.hpp"

SocketWaiter SocketClient::invoke(std::string path, RequestInvoke request, RequestCallback callback) {
  SocketWaiter waiter = std::make_shared<SocketWaiter_t>();
  this->socket->pushWaiter(waiter);

  TcpRequest::regist(path, callback);

  auto protocal = ProtocalFactory::getProtocal();
  auto outputStream = ProtocalFactory::createStream();

  protocal->setHeader(outputStream.get(), path);

  request(outputStream.get());

  protocal->setLength(outputStream.get());

  Antelope::instance.send(this->socket, outputStream);

  return waiter;
}

SocketWaiter SocketClient::invoke(std::string path, void* buffer, int len, RequestCallback callback) {
  SocketWaiter waiter = std::make_shared<SocketWaiter_t>();
  this->socket->pushWaiter(waiter);

  leopard_debug("< invoke %s", path.c_str());
  TcpRequest::regist(path, callback);

  auto protocal = ProtocalFactory::getProtocal();
  auto outputStream = ProtocalFactory::createStream();

  protocal->setHeader(outputStream.get(), path);

  outputStream->put<uint32_t>(len);
  outputStream->puts(buffer, len);

  protocal->setLength(outputStream.get());

  Antelope::instance.send(this->socket, outputStream);

  return waiter;
}

#ifdef LEOPARD_SUSPEND
// error: invalid new-expression of abstract class type ‘BufferStream’
await<BufferStream*> SocketClient::invoke(std::string path, void* buffer, int len) {
  Socket* socket = this->this->socket;
  auto waiter = await<BufferStream*>([&path, len, buffer, &socket](suspend::SuspendHandler handle, BufferStream** returnValue) {
    logger_warn(path.c_str());
    char* str = (char*)buffer;
    logger_warn(str);
    SuspendCallback callback;
    callback.handle = handle;
    callback.returnValue = returnValue;
    TcpRequest::registSuspend(path, callback);

    //================================================================
    auto protocal = ProtocalFactory::getProtocal();
    auto outputStream = ProtocalFactory::createStream();

    protocal->setHeader(outputStream.get(), path);

    outputStream->put<uint32_t>(len);
    outputStream->puts(buffer, len);

    protocal->setLength(outputStream.get());

    task = new SendTask(this->socket, outputStream);
    Antelope::instance.send(task);
  });
  return waiter;
}
#endif

void SocketClient::close() {
  this->socket->close();
}

SocketClientPtr SocketClient::create(const char* ip, int port) {
  ClientSocket* socket = Antelope::instance.create(ip, port);
  auto client = std::make_shared<SocketClient>(socket);
  return client;
}