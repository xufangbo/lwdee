#include "Runway.hpp"

#include "TcpResponse.hpp"
#include "core/Exception.hpp"
#include "core/Stopwatch.hpp"
#include "core/log.hpp"
#include "net/ProtocalFactory.hpp"
#include "net/log.hpp"
#include "sys/sysinfo.h"

Runway::Runway(int id, bool* running, std::string ip, int port)
    : IRunway(id, running), ip(ip), port(port) {
}

void Runway::start() {
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
    IRunway::__acceptEvent(evt);
  }
}

void Runway::acceptSocket(epoll_event* evt) {
  if (evt->events & EPOLLIN) {
    int client_fd = server->accept();

    Socket* client = new Socket(client_fd, &_qps);
    client->setReciveBuf(BUFFER_SIZE);

    connections->insert(client);

    client->setNonBlocking();

    uint32_t events = EPOLLIN;
    events |= (EPOLLRDHUP | EPOLLHUP);
    if (isEOUT) {
      events |= EPOLLOUT;
    }
    if (isET) {
      events |= EPOLLET;
    }
    epoll->add(client_fd, events);

    // leopard_trace("socket accept %d", client_fd);

  } else if (evt->events & EPOLLIN) {
    leopard_info("server socket EPOLLOUT");
  } else {
    leopard_info("server socket unkown event %d", evt->events);
  }
}

void Runway::__acceptRequest(Connection* connection, BufferStreamPtr inputStream) {
  auto header = this->parseRequest(inputStream.get());

  auto fun = TcpResponse::find(header->path);
  if (fun == nullptr) {
    logger_error("can't hint path: %s", header->path.c_str());
  } else {
    auto outputStream = ProtocalFactory::createStream();
    auto protocal = ProtocalFactory::getProtocal();

    uint32_t rec1 = Stopwatch::currentMilliSeconds() - header->sen1;
    protocal->setHeader(outputStream.get(), header->path, header->sen1, rec1, 0, 0);

    (*fun)(inputStream.get(), outputStream.get());

    uint32_t sen2 = Stopwatch::currentMilliSeconds() - header->sen1;
    protocal->setsen2(outputStream.get(), sen2);
    protocal->setLength(outputStream.get());

    // sendQueue->push(socket, outputStream);
    this->addSendTask(connection, outputStream);

    // leopard_debug("> response %s", header->path.c_str());
  }
}
