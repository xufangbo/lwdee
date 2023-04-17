#include "SocketClient.hpp"
#include "Antelope.hpp"
#include "core/Exception.hpp"
#include "core/log.hpp"
#include "core/suspend.hpp"
#include "net/Epoll.hpp"
#include "net/LeopardConfig.hpp"
#include "net/Socket.hpp"

#define MAX_EPOLLSIZE (384 * 1024)

#ifdef LEOPARD_SUSPEND
suspend testSuspend(SocketClient* client, int i) ;
#endif

void testCallback(SocketClient* client, int i) ;
void testBigDataCallback(SocketClient* client, int i) ;

int main(int argc, char** argv) {
  
  read_log_config("client");

  auto* conf = LeopardConfig::instance();
  conf->readConfig();
  logger_trace("%s:%d", conf->ip.c_str(), conf->port);

  Antelope::instance.start();

  for (int i = 0; i < 10; i++) {
    try {
      auto client = SocketClient::create(conf->ip.c_str(), conf->port);

      // testCallback(client.get(), i);
      testBigDataCallback(client.get(), i);

      // sleep(1);

    } catch (Exception& ex) {
      logger_warn("%s", ex.getMessage().c_str());
    } catch (std::exception& ex) {
      logger_error("%s", ex.what());
    }

    usleep(1000000 / 100);
  }

  sleep(20);

  return 0;
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

  client->wait();
}

void testBigDataCallback(SocketClient* client, int i) {
  std::string input = "green green !";
  for (int i = 0; i < 50000;i++){
    input += "green green !";
  }
  input += std::to_string(i);
  logger_debug("send %s", input.c_str());

  RequestCallback callback = [](BufferStream* inputStream) {
    auto len = inputStream->get<uint32_t>();
    auto content = inputStream->getString(len);

    logger_info("recive(%d) :  %s", len, content.c_str());
  };

  client->invoke("com.cs.sales.order.save", (void*)input.c_str(), input.size(), callback);

  auto time = client->wait();
  logger_info("%d eclipse %lfs",i, time * 1.0 / 1000);
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
