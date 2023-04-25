#include "LaneClient.hpp"

#include "Leopard.hpp"
#include "core/Exception.hpp"
#include "core/Stopwatch.hpp"
#include "core/log.hpp"
#include "src/IProtocal.hpp"
#include "src/ProtocalFactory.hpp"
#include "src/log.hpp"

LaneClient::~LaneClient() {
  for (auto connection : this->connections) {
    if (connection != nullptr) {
      connection->close(CloseType::normal);
    }
  }
}

ClientWaitor LaneClient::invoke(std::string path, void* buffer, int len, RequestCallback callback) {
  RequestInvoke request = [buffer, len](BufferStream* outputStream) {
    outputStream->put<uint32_t>(len);
    outputStream->puts(buffer, len);
  };

  return invoke(path, request, callback);
}

ClientWaitor LaneClient::invoke(std::string path, RequestInvoke request, RequestCallback callback) {
  ClientConnection* connection = this->next();

  TcpRequest::regist(path, callback);

  auto protocal = ProtocalFactory::getProtocal();

  auto outputStream = protocal->newStream(path);
  request(outputStream);

  auto msgId = protocal->csend(connection, outputStream);

  auto waiter = connection->crateWaiter(msgId);
  waiter->client = this;

  this->waiters.push_back(waiter);

  return waiter;
}

void LaneClient::onInvoked(ClientConnection* connection) {
  if (connection == nullptr) {
    return;
  }

  if (!autoClose) {
    return;
  }
  auto it = find(connections.begin(), connections.end(), connection);
  if (it == connections.end()) {
    return;
  }

  std::lock_guard lock(mut);
  this->connections.erase(it);

  if (*it == nullptr) {
    return;
  }

   leopard_debug("connection closed");
  connection->close(CloseType::normal);
}

void LaneClient::wait(float timeout) {
  for (auto waiter : waiters) {
    waiter->wait(timeout);
  }
}
void LaneClient::close() {
  for (ClientConnection* connection : this->connections) {
    connection->close(CloseType::normal);
  }

  std::lock_guard lock(mut);
  this->connections.clear();
}

ClientConnection* LaneClient::next() {
  int i = this->index % connections.size();
  ClientConnection* connection = this->connections[i];
  this->index = (i + 1);

  return connection;
}

void LaneClient::addConnection(ClientConnection* connection){
  std::lock_guard lock(mut);
  this->connections.push_back(connection);
}

LaneClientPtr LaneClient::create(std::string ip, int port, int parallel) {
  auto client = std::make_shared<LaneClient>();

  for (int i = 0; i < parallel; i++) {
    ClientConnection* connection = Leopard::instance.create(ip, port);

    client->addConnection(connection);
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

        task = new ClientConnection(this->socket, outputStream);
        Leopard::instance.send(task);
      });
  return waiter;
}
#endif
