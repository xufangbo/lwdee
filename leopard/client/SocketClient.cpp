#include "SocketClient.hpp"

#include "Antelope.hpp"
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

  Antelope::instance.send(_socket, outputStream);
}

void SocketClient::invoke(std::string path, void* buffer, int len, RequestCallback callback) {
#ifdef LEOPARD_TRACING
  logger_trace("< invoke %s", path.c_str());
#endif
  TcpRequest::regist(path, callback);

  auto protocal = ProtocalFactory::getProtocal();
  auto outputStream = ProtocalFactory::createStream();

  protocal->setHeader(outputStream.get(), path);

  outputStream->put<uint32_t>(len);
  outputStream->puts(buffer, len);

  protocal->setLength(outputStream.get());

  Antelope::instance.send(_socket, outputStream);
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

    Antelope::send(socket, outputStream->buffer, outputStream->size());
  });
  return waiter;
}
#endif

double SocketClient::wait(int timeout) {
  Stopwatch sw;
  int fd = this->_socket->fd();
  for (int i = 0; i < 100 * timeout; i++) {
    bool ok = Antelope::instance.contains(fd);
    if (!ok) {
      return sw.stop();
    }
    usleep(1000000 / 100);
  }
  throw Exception("timeout", ZONE);
}

Socket* SocketClient::socket() {
  return this->_socket;
}

SocketClientPtr SocketClient::create(const char* ip, int port) {
  Socket* socket = Antelope::instance.create(ip, port);
  auto client = std::make_shared<SocketClient>(socket);
  return client;
}