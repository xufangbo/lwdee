#include "SocketClient.hpp"

#include "SocketScheduler.hpp"
#include "core/Exception.hpp"
#include "core/Stopwatch.hpp"
#include "core/log.hpp"
#include "net/LeopardProtocal.hpp"
#include "net/ProtocalFactory.hpp"

void SocketClient::invoke(std::string path, RequestInvoke request, RequestCallback callback) {
  TcpRequest::regist(path, callback);

  auto protocal = ProtocalFactory::getProtocal();
  auto outputStream = ProtocalFactory::createStream();

  protocal->setHeader(outputStream.get(), path);

  request(outputStream.get());

  protocal->setLength(outputStream.get());

  SocketScheduler::send(_socket, outputStream->buffer, outputStream->size());
}

void SocketClient::invoke(std::string path, void* buffer, int len, RequestCallback callback) {
  logger_trace("< invoke %s",path.c_str());
  TcpRequest::regist(path, callback);

  auto protocal = ProtocalFactory::getProtocal();
  auto outputStream = ProtocalFactory::createStream();

  protocal->setHeader(outputStream.get(), path);

  outputStream->put<uint32_t>(len);
  outputStream->puts(buffer, len);

  protocal->setLength(outputStream.get());

  SocketScheduler::send(_socket, outputStream->buffer, outputStream->size());
}

#ifdef LEOPARD_SUSPEND
// error: invalid new-expression of abstract class type ‘BufferStream’
await<BufferStream*> SocketClient::invoke(std::string path, void* buffer, int len) {
  Socket* socket = this->_socket;
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

    SocketScheduler::send(socket, outputStream->buffer, outputStream->size());
  });
  return waiter;
}
#endif

void SocketClient::wait() {
  int fd = this->_socket->fd();
  for (int i = 0; i < 100 * 10; i++) {
    bool connected = SocketScheduler::contains(fd);
    if (!connected) {
      return;
    }
    usleep(1000000 / 100);
  }
  throw Exception("timeout", ZONE);
}

Socket* SocketClient::socket() {
  return this->_socket;
}