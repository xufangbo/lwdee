#include "Runway.hpp"

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

    leopard_debug("server socket sendbufer %d", server->getSendBuf());    // 425984
    leopard_debug("server socket revibufer %d", server->getReciveBuf());  // 131072

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
    logger_error("%s,%s:%d", ex.what(), __FILE__, __LINE__);
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

    leopard_debug("client socket sendbufer %d", client->getSendBuf());    // 425984
    leopard_debug("client socket revibufer %d", client->getReciveBuf());  // 131072

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

void Runway::__acceptRequest(Connection* connection, BufferStream* inputStream) {
  auto protocal = ProtocalFactory::getProtocal();
  protocal->saccept(this, connection, inputStream);
}
