#include "Antelope.hpp"
#include "SocketClient.hpp"
#include "core/Exception.hpp"
#include "core/log.hpp"
#include "core/suspend.hpp"
#include "net/Epoll.hpp"
#include "net/LeopardConfig.hpp"
#include "net/Socket.hpp"
#include "test.hpp"

#define MAX_EPOLLSIZE (384 * 1024)

#ifdef LEOPARD_SUSPEND
suspend testSuspend(SocketClient* client, int i);
#endif




int main(int argc, char** argv) {
  for (int i = 0; i < 30; i++) {
    printf("\n");
  }
  read_log_config("client");

  auto* conf = LeopardConfig::instance();
  conf->readConfig();
  logger_trace("%s:%d", conf->ip.c_str(), conf->port);

  Antelope::instance.start();

  // responseIndex = 0;
  // for (int i = 0; i < 1000; i++) {
  //   logger_trace("----------------------------");
  //   try {
  //     auto client = SocketClient::create(conf->ip.c_str(), conf->port, 1);

  //     // testLongConnection(client.get(), i);
  //     testCallback(client.get(), i);
  //     // testBigDataCallback(client.get(), i);

  //     // client->wait();
  //     // client->close();

  //   } catch (Exception& ex) {
  //     logger_warn("%s", ex.getMessage().c_str());
  //   } catch (std::exception& ex) {
  //     logger_error("%s", ex.what());
  //   }
  // }

  // test_1000_small_short_sync(conf->ip, conf->port);
  test_1000_small_short_async(conf->ip, conf->port);
  Antelope::instance.join();

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
