// #include <arpa/inet.h>
// #include <errno.h>
// #include <fcntl.h>
// #include <netdb.h>
// #include <netinet/tcp.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/epoll.h>
// #include <sys/socket.h>
// #include <sys/time.h>
// #include <sys/types.h>

// #include <thread>

#include "SocketClient.hpp"
#include "SocketScheduler.hpp"
#include "core/Exception.hpp"
#include "core/log.hpp"
#include "core/suspend.hpp"
#include "net/Epoll.hpp"
#include "net/LeopardConfig.hpp"
#include "net/Socket.h"

#define MAX_EPOLLSIZE (384 * 1024)

suspend testSuspend(SocketClient* client, int i) {
  std::string input = "green green green " + std::to_string(i);
  auto path = "com.cs.sales.order.save";

  logger_debug("send %s", input.c_str());
  BufferStream* inputStream = co_await client->invoke(path, (void*)input.c_str(), input.size());

  auto len = inputStream->get<uint32_t>();
  auto content = inputStream->getString(len);

  logger_info("recive(%d) :  %s", len, content.c_str());
}

void testCallback(SocketClient* client, int i) {
  std::string input = "green green green ";
  input += std::to_string(i);
  logger_debug("send %s", input.c_str());

  RequestCallback callback = [](BufferStream* inputStream) {
    auto len = inputStream->get<uint32_t>();
    auto content = inputStream->getString(len);

    logger_info("recive(%d) :  %s", len, content.c_str());
  };

  client->invoke("com.cs.sales.order.save", (void*)input.c_str(), input.size(), callback);

  // client->wait();
}

void init_logger(std::string nodeName);

int main(int argc, char** argv) {
  init_logger("client");

  auto* conf = LeopardConfig::instance();
  conf->readConfig();
  logger_trace("%s:%d", conf->ip.c_str(), conf->port);

  SocketScheduler::start();

  for (int i = 0; i < 1; i++) {
    // SocketClient::tracing = true;

    try {
      auto client = SocketScheduler::newClient(conf->ip.c_str(), conf->port);

      testCallback(client.get(), i);

      // sleep(1);

    } catch (Exception& ex) {
      logger_warn("%s", ex.getMessage().c_str());
    } catch (std::exception& ex) {
      logger_error("%s", ex.what());
    }

    usleep(1000000 / 100);
  }

  SocketScheduler::join();
  // sleep(10);

  return 0;
}

void init_logger(std::string nodeName) {
  LogOption option;
  option.initalized = false;
  option.level = log_debug;
  strcpy(option.path, "./log");
  strcpy(option.name, nodeName.c_str());
  option.is_color = true;
  option.days = 10;
  option.writeFile = true;

  if (logger_initialize(option) != 0) {
    printf("log initialize error");
  } else {
    logger_info("-- app starting ... ");
  }
}

// client->invoke(
//     "com.cs.sales.order.save",
//     [](BufferStream* outputStream) {
//       std::string input = "white1 white2 white3 !";
//       outputStream->put<uint32_t>(input.size());
//       outputStream->put(input);
//       logger_warn("send %s", input.c_str());
//     },
//     [](BufferStream* inputStream) {
//       auto len = inputStream->get<uint32_t>();
//       auto content = inputStream->getString(len);

//       logger_info("recive(%d) :  %s", len, content.c_str());
//     });
