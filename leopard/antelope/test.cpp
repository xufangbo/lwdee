#include "test.hpp"

#include <atomic>
#include <sstream>
#include "Antelope.hpp"
#include "LaneClient.hpp"
#include "LaneClients.hpp"
#include "core/Exception.hpp"
#include "core/log.hpp"
#include "core/suspend.hpp"
#include "net/Epoll.hpp"
#include "net/LeopardConfig.hpp"
#include "net/Socket.hpp"

std::atomic<int> responseIndex = 0;
std::string path = "com.cs.sales.order.save";

std::string input_small(int i) {
  std::string input = std::to_string(i + 1) + " green green green ";
  // logger_debug("send [%i]", i + 1);
  return input;
}

std::string input_large(int i) {
  std::string input = std::to_string(i + 1) + " green green green ";
  for (int x = 0; x < 50000; x++) {
    input += "green---!";
  }
  logger_debug("send [%i]", i + 1);
  return input;
}

RequestCallback callback = [](BufferStream* inputStream) {
  responseIndex++;
  auto len = inputStream->get<uint32_t>();
  // auto content = inputStream->getString(len);

  std::istringstream s(inputStream->getString(len));
  int index;
  s >> index;

  // logger_trace("recive [%d]", index);
  printf("recive [callback:%d/content:%d]\n", responseIndex.load(), index);
};

typedef std::function<std::string(int i)> InputType;

void test_short_sync(int testSize, InputType inputType, std::string ip, int port) {
  Stopwatch sw;
  for (int i = 0; i < testSize; i++) {
    try {
      auto client = LaneClient::create(ip.c_str(), port);

      auto input = inputType(i);
      SocketWaiter waiter = client->invoke(path, (void*)input.c_str(), input.size(), callback);

      auto time = waiter->wait(100);
      client->close();
      // logger_info("wait %d eclipse %.3lfs ---------------------------------------", i + 1, time);

    } catch (Exception& ex) {
      logger_warn("%s", ex.getMessage().c_str());
    } catch (std::exception& ex) {
      logger_error("%s,%s:%d", ex.what(),__FILE__,__LINE__);
    }
  }

  logger_info("elapsed %.3lf", sw.stop());
}
void test_short_async(int testSize, InputType inputType, std::string ip, int port) {
  Stopwatch sw;

  LaneClients clients;
  for (int i = 0; i < testSize; i++) {
    try {
      auto client = clients.create(ip.c_str(), port);

      auto input = inputType(i);
      client->invoke(path, (void*)input.c_str(), input.size(), callback);

    } catch (Exception& ex) {
      logger_warn("%s", ex.getMessage().c_str());
    } catch (std::exception& ex) {
      logger_error("%s,%s:%d", ex.what(),__FILE__,__LINE__);
    }
  }

  clients.wait();
  clients.close();

  logger_info("elapsed %.3lf", sw.stop());
}

void test_long_sync(int testSize, InputType inputType, int parallel, std::string ip, int port) {
  Stopwatch sw;

  auto client = LaneClient::create(ip.c_str(), port, parallel);
  for (int i = 0; i < testSize; i++) {
    try {
      auto input = inputType(i);
      client->invoke(path, (void*)input.c_str(), input.size(), callback);
    } catch (Exception& ex) {
      logger_warn("%s", ex.getMessage().c_str());
    } catch (std::exception& ex) {
      logger_error("%s,%s:%d", ex.what(), __FILE__, __LINE__);
    }
  }

  client->wait();
  client->close();

  logger_info("elapsed %.3lf", sw.stop());
}
// void test_long_async(int testSize, InputType inputType, int parallel, std::string ip, int port) {
//   Stopwatch sw;

//   LaneClients clients;
//   for (int i = 0; i < testSize; i++) {
//     try {
//       auto client = clients.create(ip.c_str(), port);

//       auto input = inputType(i);
//       client->invoke(path, (void*)input.c_str(), input.size(), callback);

//     } catch (Exception& ex) {
//       logger_warn("%s", ex.getMessage().c_str());
//     } catch (std::exception& ex) {
//       logger_error("%s", ex.what());
//     }
//   }

//   clients.wait();
//   clients.close();

//   logger_info("elapsed %.3lf", sw.stop());
// }
