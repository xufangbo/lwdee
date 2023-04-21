#include "LaneClient.hpp"

#include "Antelope.hpp"
#include "core/Exception.hpp"
#include "core/Stopwatch.hpp"
#include "core/log.hpp"
#include "net/IProtocal.hpp"
#include "net/ProtocalFactory.hpp"
#include "net/log.hpp"

SocketWaiter LaneClient::invoke(std::string path, void* buffer, int len, RequestCallback callback) {
  RequestInvoke request = [buffer, len](BufferStream* outputStream) {
    outputStream->put<uint32_t>(len);
    outputStream->puts(buffer, len);
  };

  return invoke(path, request, callback);
}

std::atomic<uint32_t> waitId = 0;

SocketWaiter LaneClient::invoke(std::string path, RequestInvoke request, RequestCallback callback) {
  Connection* connection = this->next();
  ClientSocket* socket = (ClientSocket*)connection->socket;

  waitId++;
  SocketWaiter waiter = socket->crateWaiter(waitId.load());
  this->waiters.push_back(waiter);

  TcpRequest::regist(path, callback);

  auto protocal = ProtocalFactory::getProtocal();

  auto outputStream = protocal->newStream(path);
  request(outputStream);
  
  auto runway = Antelope::instance.selectRunway();

  protocal->send(runway,connection,outputStream);

  return waiter;
}

void LaneClient::wait() {
  for (Connection* connection : this->connections) {
    ClientSocket* socket = (ClientSocket*)connection->socket;
    if (socket->hasWaiter()) {
      logger_error("stil have waiter");
    }
  }

  for (auto waiter : waiters) {
    waiter->wait();
  }
}
void LaneClient::close() {
  for (Connection* connection : this->connections) {
    ClientSocket* socket = (ClientSocket*)connection->socket;
    socket->getLane()->close(connection);
  }
}

Connection* LaneClient::next() {
  int i = this->index % this->parallel;
  Connection* socket = this->connections[i];
  this->index = (i + 1);

  return socket;
}

LaneClientPtr LaneClient::create(const char* ip, int port, int parallel) {
  std::vector<Connection*> connections;
  for (int i = 0; i < parallel; i++) {
    Connection* connection = Antelope::instance.create(ip, port);
    connections.push_back(connection);
  }

  auto client = std::make_shared<LaneClient>(connections);
  return client;
}

#ifdef LEOPARD_SUSPEND
// error: invalid new-expression of abstract class type ‘BufferStream’
await<BufferStream*> LaneClient::invoke(std::string path, void* buffer, int len) {
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
