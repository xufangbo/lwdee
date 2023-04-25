#include "Runway.hpp"

#include <memory>
#include <thread>

#include "ClientConnection.hpp"
#include "core/Exception.hpp"
#include "core/Stopwatch.hpp"
#include "src/log.hpp"

Runway::Runway(int id, bool* running)
    : _qps(id), appType(ApplicationType::client), running(running) {
  this->epoll = std::make_shared<Epoll>(1800);
  this->isET = true;
  this->isEOUT = true;
}

Runway::Runway(int id, bool* running, std::string ip, int port)
    : Runway(id, running) {
  appType = ApplicationType::server;
  this->server.ip = ip;
  this->server.port = port;
}

Runway::~Runway() {}

void Runway::start() {
  this->thread = std::thread(&Runway::run, this);
}

void Runway::run() {
  if (appType == ApplicationType::client) {
    this->__run();
    return;
  }

  try {
    this->server.socket = new Socket(&_qps);
    _qps.opens--;

    // leopard_debug("server socket sendbufer %d", server->getSendBuf());    // 2626560
    // leopard_debug("server socket revibufer %d", server->getReciveBuf());  // 131072

    this->server.socket->reusePort();
    this->server.socket->setNonBlocking();

    this->server.socket->bind(server.ip, server.port);
    this->server.socket->listen();

    epoll->add(this->server.socket->fd(), isET ? (EPOLLIN | EPOLLET) : EPOLLIN, server.socket);

  } catch (EpollException& ex) {
    logger_error("%s", ex.getMessage().c_str());
  } catch (SocketException& ex) {
    logger_error("%s", ex.getMessage().c_str());
  } catch (std::exception& ex) {
    logger_error("%s,%s:%d", ex.what(), __FILE__, __LINE__);
  }

  this->__run();
}

void Runway::__run() {
  this->gererateEnvents();

  while (*running) {
    int waits = epoll->wait(1000);
    for (int i = 0; i < waits; i++) {
      try {
        auto evt = epoll->events(i);

        if (evt.data.ptr == server.socket) {
          this->acceptSocket(&evt);
        } else {
          this->__acceptEvent(&evt);
        }
      } catch (Exception& ex) {
        logger_error("%s %s", ex.getMessage().c_str(), ex.getStackTrace().c_str());
      } catch (std::exception& ex) {
        logger_error("%s,%s:%d", ex.what(), __FILE__, __LINE__);
      }
    }
  }
}

void Runway::acceptSocket(epoll_event* evt) {
  if (evt->events & EPOLLIN) {
    int client_fd = server.socket->accept();
    // leopard_debug("client socket sendbufer %d", client->getSendBuf());    // 425984 2626560
    // leopard_debug("client socket revibufer %d", client->getReciveBuf());  // 131072 131072

    auto connection = new Connection(client_fd, this);

    epoll->add(client_fd, EVENTS_NEW, connection);
  } else if (evt->events & EPOLLIN) {
    leopard_info("server socket EPOLLOUT");
  } else {
    leopard_info("server socket unkown event %d", evt->events);
  }
}

void Runway::__acceptEvent(epoll_event* evt) {
  // logger_debug("__acceptEvent,fd:%d", evt->data.fd);

  Connection* connection = (Connection*)evt->data.ptr;
  connection->lastTime = Stopwatch::currentMilliSeconds();

  auto socket = connection->socket;

  if (evt->events & EPOLLIN) {
    connection->acceptRecive(evt);
  } else if (evt->events & EPOLLOUT) {
    this->acceptSend(connection);
  } else if (evt->events & EPOLLHUP) {
    connection->close(CloseType::normal);
  } else if (evt->events & EPOLLRDHUP) {
    connection->close(CloseType::normal);
  } else if (evt->events & EPOLLERR) {
    connection->close(CloseType::normal);
  } else {
    leopard_warn("unkonw epoll events : %d", evt->events);
  }
}

void Runway::epoll_mod_in(int fd, struct epoll_event* event, void* ptr) {
  epoll->mod(fd, event, EVENTS_IN, ptr);
}

void Runway::epoll_mod_out(int fd, struct epoll_event* event, void* ptr) {
  epoll->mod(fd, event, EVENTS_OUT, ptr);
}

void Runway::epoll_del(int fd) {
  epoll->del(fd);
}

void Runway::acceptSend(Connection* connection) {
  connection->writable(true);
  connection->send(SendSource::epoll_out);
}

void Runway::gererateEnvents() {
  EVENTS_NEW = EPOLLIN | EPOLLRDHUP | EPOLLHUP;
  EVENTS_IN = EPOLLIN | EPOLLRDHUP | EPOLLHUP;
  EVENTS_OUT = EPOLLIN | EPOLLRDHUP | EPOLLHUP;

  if (isEOUT) {
    EVENTS_NEW |= EPOLLOUT;
    EVENTS_IN |= EPOLLOUT;
    EVENTS_OUT |= EPOLLOUT;
  }
  if (isET) {
    EVENTS_NEW |= EPOLLET;
    EVENTS_IN |= EPOLLET;
    EVENTS_OUT |= EPOLLET;
  }
}

void Runway::join() {
  thread.join();
}

Qps* Runway::qps() {
  return &_qps;
}

ClientConnection* Runway::create(std::string ip, int port) {
  ClientConnection* connection = new ClientConnection(this);
  connection->connect(ip, port);

  epoll->add(connection->fd(), EVENTS_NEW, connection);

  return connection;
}