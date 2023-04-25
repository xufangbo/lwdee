#include "LaneClient.hpp"

#include "Leopard.hpp"
#include "core/Exception.hpp"
#include "core/Stopwatch.hpp"
#include "core/log.hpp"
#include "src/IProtocal.hpp"
#include "src/ProtocalFactory.hpp"
#include "src/log.hpp"

LaneClient::~LaneClient() {
  for(auto connection : this->connections){
    if(!connection->closed){
      leopard_error("connection not closed");
    }
  }
}

SocketWaiter LaneClient::invoke(std::string path, void* buffer, int len, RequestCallback callback) {
  RequestInvoke request = [buffer, len](BufferStream* outputStream) {
    outputStream->put<uint32_t>(len);
    outputStream->puts(buffer, len);
  };

  return invoke(path, request, callback);
}

SocketWaiter LaneClient::invoke(std::string path, RequestInvoke request, RequestCallback callback) {
  Connection* connection = this->next();
  ClientSocket* socket = (ClientSocket*)connection->socket;

  TcpRequest::regist(path, callback);

  auto protocal = ProtocalFactory::getProtocal();

  auto outputStream = protocal->newStream(path);
  request(outputStream);

  SocketWaiter waiter = protocal->csend(connection, outputStream);
  this->waiters.push_back(waiter);

  return waiter;
}

void LaneClient::wait(float timeout) {
  for (auto waiter : waiters) {
    waiter->wait(timeout);
  }
}
void LaneClient::close() {
  for (Connection* connection : this->connections) {
    connection->close(CloseType::normal);
  }
}

Connection* LaneClient::next() {
  int i = this->index % connections.size();
  Connection* connection = this->connections[i];
  this->index = (i + 1);

  return connection;
}

LaneClientPtr LaneClient::create(std::string ip, int port, int parallel) {
  auto client = std::make_shared<LaneClient>();

  for (int i = 0; i < parallel; i++) {
    Connection* connection = Leopard::instance.create(ip, port);
    client->getConnections()->push_back(connection);
    usleep(1000 * 100);
  }

  return client;
}

#ifdef LEOPARD_SUSPEND
// error: invalid new-expression of abstract class type ‘BufferStream’
await<BufferStream*> LaneClient::invoke(std::string path, void* buffer, int len) {
  Socket* socket = this->this->socket;
  auto waiter = await<BufferStream*>(
      [&path, len, buffer, &socket](suspend::SuspendHandler handle,
                                    BufferStream** returnValue) {
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
        Leopard::instance.send(task);
      });
  return waiter;
}
#endif
