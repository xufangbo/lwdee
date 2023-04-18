#include "Antelope.hpp"
#include "SocketClient.hpp"
#include "core/Exception.hpp"
#include "core/log.hpp"
#include "core/suspend.hpp"
#include "net/Epoll.hpp"
#include "net/LeopardConfig.hpp"
#include "net/Socket.hpp"

#define MAX_EPOLLSIZE (384 * 1024)

#ifdef LEOPARD_SUSPEND
suspend testSuspend(SocketClient* client, int i);
#endif

void testLongConnection(SocketClient* client, int i);
void testCallback(SocketClient* client, int i);
void testBigDataCallback(SocketClient* client, int i);

int responseIndex = 0;
int main(int argc, char** argv) {
  for (int i = 0; i < 20; i++) {
    printf("\n");
  }
  read_log_config("client");

  auto* conf = LeopardConfig::instance();
  conf->readConfig();
  logger_trace("%s:%d", conf->ip.c_str(), conf->port);

  Antelope::instance.start();

  responseIndex = 0;
  for (int i = 0; i < 1; i++) {
    logger_trace("----------------------------");
    try {
      auto client = SocketClient::create(conf->ip.c_str(), conf->port, 1);

      testLongConnection(client.get(), i);
      // testCallback(client.get(), i);
      // testBigDataCallback(client.get(), i);

      // client->wait();
      // client->close();

    } catch (Exception& ex) {
      logger_warn("%s", ex.getMessage().c_str());
    } catch (std::exception& ex) {
      logger_error("%s", ex.what());
    }
  }
  Antelope::instance.join();

  return 0;
}

void testLongConnection(SocketClient* client, int i) {
  for (int i = 0; i < 10; i++) {
    logger_debug("---------");
    // testCallback(client, i);
    testBigDataCallback(client, i);
    }
}

void testCallback(SocketClient* client, int i) {
  std::string input = "green green green " + std::to_string(i);
  logger_debug("send %s", input.c_str());

  RequestCallback callback = [i](BufferStream* inputStream) {
    responseIndex++;
    auto len = inputStream->get<uint32_t>();
    auto content = inputStream->getString(len);

    logger_info("recive-%d:  (%d)%s", responseIndex, len, content.c_str());
  };

  auto waiter = client->invoke("com.cs.sales.order.save", (void*)input.c_str(), input.size(), callback);

  auto time = waiter->wait();
  logger_info("%d eclipse %.3lfs", i, time);
}

void testBigDataCallback(SocketClient* client, int i) {
  std::string input = "green green !";
  for (int i = 0; i < 50000; i++) {
    input += "green green !";
  }
  input += std::to_string(i);
  logger_debug("send %s", input.c_str());

  RequestCallback callback = [i](BufferStream* inputStream) {
    responseIndex++;
    auto len = inputStream->get<uint32_t>();
    auto content = inputStream->getString(len);

    logger_info("recive-%d:  (%d)%s", responseIndex, len, content.c_str());
  };

  auto waiter = client->invoke("com.cs.sales.order.save", (void*)input.c_str(), input.size(), callback);

  // auto time = waiter->wait();
  // logger_info("%d eclipse %.3lfs", i, time);
}

#ifdef LEOPARD_SUSPEND
suspend testSuspend(SocketClient* client, int i) {
  std::string input = "green green green " + std::to_string(i);
  auto path = "com.cs.sales.order.save";

  logger_debug("send %s", input.c_str());
  BufferStream* inputStream = co_await client->invoke(path, (void*)input.c_str(), input.size());

  auto len = inputStream->get<uint32_t>();
  auto content = inputStream->getString(len);

  logger_info("recive(%d) :  %s", len, content.c_str());
}
#endif

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
