#include "Runway.hpp"

#include "TcpResponse.hpp"
#include "core/Exception.hpp"
#include "core/log.hpp"
#include "net/LeopardProtocal.hpp"
#include "net/ProtocalFactory.hpp"
#include "sys/sysinfo.h"

#define BUFFER_SIZE 1024

Runway::Runway() {
  this->epoll = new Epoll(1000);
}
Runway::~Runway() {
  if (epoll != nullptr) {
    delete epoll;
    epoll = nullptr;
  }
}

void Runway::start(std::string ip, int port, int index) {
  this->ip = ip;
  this->port = port;
  this->index = index;

  this->thread = std::thread(&Runway::run, this);
}

void Runway::run() {
  epoll = new Epoll(18000);

  try {
    this->server = std::make_shared<Socket>();

    this->server->reusePort();
    this->server->setNonBlocking();
    this->server->bind(ip.c_str(), port);
    this->server->listen();

    epoll->add(this->server->fd(), isET ? (EPOLLIN | EPOLLET) : EPOLLIN);

    logger_trace("server socket fd - %d : %d", index, server->fd());

  } catch (EpollException& ex) {
    logger_error("%s", ex.getMessage().c_str());
  } catch (SocketException& ex) {
    logger_error("%s", ex.getMessage().c_str());
  } catch (std::exception& ex) {
    logger_error("%s", ex.what());
  }

  /**
   * TPS统计，系统每秒钟能够处理的业务数量
   * 是一种业务概念，不是一种标准，不同的业务人员统计口径可能不一样
   * 比如可能指的是每秒钟能保存成功的订单数
   * 这里指的是一秒钟RPC请求处理的个数
   */
  while (1) {
    _waits = epoll->wait(-1);  // 如果没有请求进来，这里会卡住
    for (int i = 0; i < _waits; i++) {
      try {
        handleEvent(i);
      } catch (EpollException& ex) {
        logger_error("%s", ex.getMessage().c_str());
      } catch (SocketException& ex) {
        logger_error("%s", ex.getMessage().c_str());
      } catch (std::exception& ex) {
        logger_error("%s", ex.what());
      }
      // usleep(1 * 1000);
    }
  }

  logger_error("runway while end");
}

void Runway::handleEvent(int i) {
  epoll_event evt = epoll->events(i);

  if (evt.data.fd == server->fd()) {
    this->accept(&evt);
  } else {
    this->io(&evt);
  }
}

void Runway::accept(epoll_event* evt) {
  if (evt->events & EPOLLIN) {
    int client_fd = server->accept();

    Socket* client = new Socket(client_fd);
    clients.insert(client);

    client->setNonBlocking();

    uint32_t events = EPOLLIN;
    events |= EPOLLOUT;
    events |= (EPOLLRDHUP | EPOLLHUP);

    epoll->add(client_fd, isET ? (events | EPOLLET) : events);

    _tps++;

    if (tracing) logger_trace("socket accept %d", client_fd);
  } else if (evt->events & EPOLLIN) {
    logger_warn("server socket  EPOLLOUT");
  } else {
    logger_warn("server socket  unkown event %d", evt->events);
  }
}

void Runway::io(epoll_event* evt) {
  auto socket = clients.find(evt->data.fd);
  if (socket == nullptr) {
    logger_debug("no hint socket %d", evt->data.fd);
    return;
  }

  if (evt->events & EPOLLIN) {
    this->recv(socket, evt);
  } else if (evt->events & EPOLLOUT) {
    epollOut = true;
    // logger_trace("EPOLL OUT do nothing");
  } else if (evt->events & EPOLLHUP) {
    logger_info("close client: EPOLLHUP %d", socket->fd());
    this->close(socket);
  } else if (evt->events & EPOLLRDHUP) {
    logger_info("close client: EPOLLRDHUP %d", socket->fd());
    this->close(socket);
  } else if (evt->events & EPOLLERR) {
    logger_info("close client : EPOLLERR %d", socket->fd());
    this->close(socket);
  } else {
    logger_warn("unkonw epoll events : %d", evt->events);
  }
}

void Runway::recv(Socket* socket, epoll_event* evt) {
  char buf[BUFFER_SIZE] = {0};
  int rc = 0;
  try {
    rc = socket->recv(buf, BUFFER_SIZE, MSG_WAITALL);
  } catch (SocketException& ex) {
    ex.trace(ZONE);
    this->close(socket);
    throw ex;
  }

  // if (tracing) logger_trace("recv - %s,rc:%d", buf, rc);

  auto* inputStream = socket->inputStream();
  inputStream->puts(buf, rc);

  if (rc == 0) {  // 对方socket close
    if (tracing) {
      logger_trace("client socket(%d) closed by read.rc = 0", socket->fd());
    }
    this->close(socket);
  } else if (inputStream->isEnd()) {
    if (tracing) {
      logger_debug("%s", inputStream->buffer);
    }

    std::thread t(&Runway::handleRequest, this, socket);
    t.detach();

  } else {
    epoll->mod(evt, socket->fd(), isET ? (EPOLLIN | EPOLLET) : (EPOLLIN));
  }
}

void Runway::handleRequest(Socket* socket) {
  try{
    this->doHandle(socket);
  }catch(Exception &ex){
    logger_error("%s %s",ex.getMessage().c_str(),ex.getStackTrace().c_str());
  }catch(std::exception &ex){
    logger_error("%s",ex.what());
  }
}
void Runway::doHandle(Socket* socket) {
  auto* inputStream = socket->inputStream();
  inputStream->reset();
  auto total_len = inputStream->get<uint64_t>();
  auto path_len = inputStream->get<uint32_t>();
  std::string path = inputStream->getString(path_len);

  logger_debug("< accept %s", path.c_str());

  auto fun = TcpResponse::find(path);
  if (fun == nullptr) {
    logger_error("> response can't hint path %s", path.c_str());
    inputStream->clean();
  } else {
    
    auto protocal = ProtocalFactory::getProtocal().get();
    auto outputStream = ProtocalFactory::createStream();

    protocal->setHeader(outputStream.get(), path);

    (*fun)(inputStream, outputStream.get());

    protocal->setLength(outputStream.get());

    inputStream->clean();

    this->send(socket, outputStream.get());

    logger_debug("> response %s", path.c_str());
  }
}

void Runway::send(Socket* socket, BufferStream* outStream) {
  // std::string response = "blue1 blue2 blue3 !";
  // if (tracing) logger_debug("send - %s", response.c_str());

  int rc = socket->send(outStream->buffer, outStream->size());
  if (rc == -1 && errno == EAGAIN) {
    logger_info("send EAGAIN error");
    // send 函数中的size变量大小超过了tcp_sendspace的值
    // epollOut = false;
  }
}

void Runway::close(Socket* socket) {
  epoll->del(socket->fd());
  socket->close();

  clients.remove(socket);
  delete socket;
}

int Runway::tps() {
  int tmp = _tps;
  _tps = 0;
  return tmp;
}

int Runway::waits() { return _waits; }

int Runway::sockets() { return clients.size(); }

void Runway::join() { thread.join(); }