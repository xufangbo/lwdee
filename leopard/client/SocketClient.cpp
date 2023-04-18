#include "SocketClient.hpp"

#include "Antelope.hpp"
#include "core/Exception.hpp"
#include "core/Stopwatch.hpp"
#include "net/LeopardProtocal.hpp"
#include "net/ProtocalFactory.hpp"
#include "net/log.hpp"

SocketWaiter SocketClient::invoke(std::string path, RequestInvoke request, RequestCallback callback) {
  ClientSocket* socket = this->next();

  SocketWaiter waiter = std::make_shared<SocketWaiter_t>();
  socket->pushWaiter(waiter);

  TcpRequest::regist(path, callback);

  auto protocal = ProtocalFactory::getProtocal();
  auto outputStream = ProtocalFactory::createStream();

  protocal->setHeader(outputStream.get(), path);

  request(outputStream.get());

  protocal->setLength(outputStream.get());

  Antelope::instance.send(socket, outputStream);

  return waiter;
}

SocketWaiter SocketClient::invoke(std::string path, void* buffer, int len, RequestCallback callback) {
  ClientSocket* socket = this->next();

  SocketWaiter waiter = std::make_shared<SocketWaiter_t>();
  socket->pushWaiter(waiter);

  leopard_debug("< invoke %s", path.c_str());
  TcpRequest::regist(path, callback);

  auto protocal = ProtocalFactory::getProtocal();
  auto outputStream = ProtocalFactory::createStream();

  protocal->setHeader(outputStream.get(), path);

  outputStream->put<uint32_t>(len);
  outputStream->puts(buffer, len);

  protocal->setLength(outputStream.get());

  Antelope::instance.send(socket, outputStream);

  return waiter;
}

void SocketClient::wait() {
  for (ClientSocket* socket : this->sockets) {
    socket->wait();
  }
}
void SocketClient::close() {
  for (ClientSocket* socket : this->sockets) {
    socket->close();
  }
}

ClientSocket* SocketClient::next() {
  int i = this->index % this->parallel;
  ClientSocket* socket = this->sockets[i];
  this->index = (i + 1);

  // leopard_debug("sockeet index %d",i);
  return socket;
}

SocketClientPtr SocketClient::create(const char* ip, int port, int parallel) {
  std::vector<ClientSocket*> sockets;
  for (int i = 0; i < parallel; i++) {
    ClientSocket* socket = Antelope::instance.create(ip, port);
    sockets.push_back(socket);
  }

  auto client = std::make_shared<SocketClient>(sockets);
  return client;
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