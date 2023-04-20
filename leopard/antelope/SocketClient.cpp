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
  ClientSocket* socket = this->next();

  this->waitId++;
  SocketWaiter waiter = std::make_shared<SocketWaiter_t>(this->waitId.load());
  socket->pushWaiter(waiter);

  TcpRequest::regist(path, callback);

  auto protocal = ProtocalFactory::getProtocal();
  auto outputStream = ProtocalFactory::createStream();

  protocal->setHeader(outputStream.get(), path, Stopwatch::currentMilliSeconds(), 0, 0, 0);

  request(outputStream.get());

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
    socket->getLan()->close(socket->fd());
  }
}

ClientSocket* SocketClient::next() {
  int i = this->index % this->parallel;
  ClientSocket* socket = this->sockets[i];
  this->index = (i + 1);

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

SocketClientPtr SocketClients::create(const char* ip, int port) {
  auto client = SocketClient::create(ip, port);
  this->clients.push_back(client);
  return client;
}

void SocketClients::wait() {
  SocketWaiter waiter;
  for (auto client : this->clients) {
    try {
      for (auto socket : client->getSockets()) {
        while ((waiter = socket->popWaiter()) != nullptr) {
          waiter->wait();
        }
      }
    } catch (Exception& ex) {
      logger_warn(" %s", ex.getMessage().c_str());
    } catch (std::exception& ex) {
      logger_error(" %s", ex.what());
    }
  }
}

void SocketClients::close() {
  for (auto client : this->clients) {
    try {
      client->close();
    } catch (Exception& ex) {
      logger_warn(" %s", ex.getMessage().c_str());
    } catch (std::exception& ex) {
      logger_error(" %s", ex.what());
    }
  }
}