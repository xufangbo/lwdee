#include "Runway.hpp"

#include "TcpResponse.hpp"
#include "core/Exception.hpp"
#include "core/Stopwatch.hpp"
#include "core/log.hpp"
#include "net/ProtocalFactory.hpp"
#include "net/log.hpp"
#include "sys/sysinfo.h"

Runway::Runway(int id, bool* running, SendTaskQueue* sendQueue)
    : IRunway(id, running, sendQueue) {
}

void Runway::start(std::string ip, int port) {
  this->ip = ip;
  this->port = port;

  this->thread = std::thread(&Runway::run, this);
}

void Runway::run() {
  try {
    this->server = std::make_shared<Socket>(&_qps);

    this->server->reusePort();
    this->server->setNonBlocking();
    this->server->bind(ip.c_str(), port);
    this->server->listen();

    epoll->add(this->server->fd(), isET ? (EPOLLIN | EPOLLET) : EPOLLIN);

  } catch (EpollException& ex) {
    logger_error("%s", ex.getMessage().c_str());
  } catch (SocketException& ex) {
    logger_error("%s", ex.getMessage().c_str());
  } catch (std::exception& ex) {
    logger_error("%s", ex.what());
  }

  IRunway::run();
}

void Runway::acceptEvent(epoll_event* evt) {
  if (evt->data.fd == server->fd()) {
    this->acceptSocket(evt);
  } else {
    IRunway::acceptRecive(evt);
  }
}

void Runway::acceptSocket(epoll_event* evt) {
  if (evt->events & EPOLLIN) {
    int client_fd = server->accept();

    Socket* client = new Socket(client_fd, &_qps);
    sockets.insert(client);

    client->setNonBlocking();

    uint32_t events = EPOLLIN;
    events |= EPOLLOUT;
    events |= (EPOLLRDHUP | EPOLLHUP);

    epoll->add(client_fd, isET ? (events | EPOLLET) : events);

    leopard_trace("socket accept %d", client_fd);

  } else if (evt->events & EPOLLIN) {
    leopard_info("server socket EPOLLOUT");
  } else {
    leopard_info("server socket unkown event %d", evt->events);
  }
}

void Runway::doAcceptRequest(Socket* socket,BufferStreamPtr inputStream) {
  auto header = this->parseRequest(inputStream.get());

  auto fun = TcpResponse::find(header->path);
  if (fun == nullptr) {
    logger_error("can't hint path: %s", header->path.c_str());
  } else {
    auto outputStream = ProtocalFactory::createStream();

    auto protocal = ProtocalFactory::getProtocal();
    protocal->setHeader(outputStream.get(), header->path);

    (*fun)(inputStream.get(), outputStream.get());

    protocal->setLength(outputStream.get());

    sendQueue->push(socket, outputStream);

    leopard_debug("> response %s", header->path.c_str());
  }
}
