#include "SocketClient.hpp"

#include "Antelope.hpp"
#include "core/Exception.hpp"
#include "core/Stopwatch.hpp"
#include "core/log.hpp"
#include "net/LeopardProtocal.hpp"
#include "net/ProtocalFactory.hpp"
#include "net/log.hpp"

SocketWaiter SocketClient::invoke(std::string path, void* buffer, int len, RequestCallback callback) {
  RequestInvoke request = [buffer, len](BufferStream* outputStream) {
    outputStream->put<uint32_t>(len);
    outputStream->puts(buffer, len);
  };

  return invoke(path, request, callback);
}

SocketWaiter SocketClient::invoke(std::string path, RequestInvoke request, RequestCallback callback) {
  Connection* connection = this->next();
  ClientSocket* socket = (ClientSocket*)connection->socket;

  this->waitId++;
  SocketWaiter waiter = std::make_shared<SocketWaiter_t>(this->waitId.load());
  socket->pushWaiter(waiter);

  TcpRequest::regist(path, callback);

  auto protocal = ProtocalFactory::getProtocal();
  auto outputStream = ProtocalFactory::createStream();

  protocal->setHeader(outputStream.get(), path, Stopwatch::currentMilliSeconds(), 0, 0, 0);

  request(outputStream.get());

  protocal->setLength(outputStream.get());

  Antelope::instance.send(connection, outputStream);

  return waiter;
}

void SocketClient::wait() {
  for (Connection* connection : this->connections) {
    ClientSocket* socket = (ClientSocket*)connection->socket;
    socket->wait();
  }
}
void SocketClient::close() {
  for (Connection* connection : this->connections) {
    ClientSocket* socket = (ClientSocket*)connection->socket;
    socket->getLane()->close(connection);
  }
}

Connection* SocketClient::next() {
  int i = this->index % this->parallel;
  Connection* socket = this->connections[i];
  this->index = (i + 1);

  return socket;
}

SocketClientPtr SocketClient::create(const char* ip, int port, int parallel) {
  std::vector<Connection*> connections;
  for (int i = 0; i < parallel; i++) {
    Connection* connection = Antelope::instance.create(ip, port);
    connections.push_back(connection);
  }

  auto client = std::make_shared<SocketClient>(connections);
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

    task = new Connection(this->socket, outputStream);
    Antelope::instance.send(task);
  });
  return waiter;
}
#endif

SocketClientPtr SocketClients::create(const char* ip, int port) {
  auto client = SocketClient::create(ip, port);
  this->clients.push_back(client);
  return client;
}

void SocketClients::wait() {
  SocketWaiter waiter;

  for (int i = 0; i < waiters.size(); i++) {
    auto waiter = this->waiters[i];
    try {
      // printf("\nwait %d\n\n", i);
      waiter->wait();
    } catch (Exception& ex) {
      logger_error("[%d] - %s", (i + 1), ex.getMessage().c_str());
    } catch (std::exception& ex) {
      logger_error("[%d] - %s", (i + 1), ex.what());
    }
  }
}

void SocketClients::close() {
  for (int i = 0; i < clients.size(); i++) {
    auto client = this->clients[i];
    try {
      client->close();
    } catch (Exception& ex) {
      logger_error("[%d] - %s", (i + 1), ex.getMessage().c_str());
    } catch (std::exception& ex) {
      logger_error("[%d] - %s", (i + 1), ex.what());
    }
  }
}