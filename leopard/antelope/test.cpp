#include "test.hpp"

#include "Antelope.hpp"
#include "SocketClient.hpp"
#include "core/Exception.hpp"
#include "core/log.hpp"
#include "core/suspend.hpp"
#include "net/Epoll.hpp"
#include "net/LeopardConfig.hpp"
#include "net/Socket.hpp"

int responseIndex = 0;
std::string path = "com.cs.sales.order.save";

std::string input_small(int i) {
  std::string input = "green green green " + std::to_string(i);
  logger_debug("send %s", input.c_str());
  return input;
}

std::string input_large(int i) {
  std::string input = "green green !";
  for (int i = 0; i < 50000; i++) {
    input += "green green !";
  }
  input += std::to_string(i);
  logger_debug("send %s", input.c_str());
  return input;
}

RequestCallback callback = [](BufferStream* inputStream) {
  responseIndex++;
  auto len = inputStream->get<uint32_t>();
  auto content = inputStream->getString(len);

  logger_debug("recive-%d:  (%d)%s", responseIndex, len, content.c_str());
};

typedef std::function<std::string(int i)> InputType;

void test_sync(int testSize, InputType inputType, std::string ip, int port) {
  Stopwatch sw;
  for (int i = 0; i < testSize; i++) {
    try {
      auto client = SocketClient::create(ip.c_str(), port, 1);

      auto input = inputType(i);
      SocketWaiter waiter = client->invoke(path, (void*)input.c_str(), input.size(), callback);

      auto time = waiter->wait();
      logger_info("wait %d eclipse %.3lfs ---------------------------------------", i, time);

      client->close();

    } catch (Exception& ex) {
      logger_warn("%s", ex.getMessage().c_str());
    } catch (std::exception& ex) {
      logger_error("%s", ex.what());
    }
  }

  logger_info("elapsed %.3lf", sw.stop());
}
void test_async(int testSize, InputType inputType, std::string ip, int port) {
  Stopwatch sw;

  SocketClients clients;
  for (int i = 0; i < testSize; i++) {
    try {
      auto client = clients.create(ip.c_str(), port);

      auto input = inputType(i);
      SocketWaiter waiter = client->invoke(path, (void*)input.c_str(), input.size(), callback);

      clients.waiters.push_back(waiter);

    } catch (Exception& ex) {
      logger_warn("%s", ex.getMessage().c_str());
    } catch (std::exception& ex) {
      logger_error("%s", ex.what());
    }
  }

  clients.wait();
  clients.close();

  logger_info("elapsed %.3lf", sw.stop());
}

void test_1000_small_short_sync(std::string ip, int port) {
  test_sync(1000, input_small, ip, port);
}

void test_1000_small_short_async(std::string ip, int port) {
  test_async(1000, input_small, ip, port);
}

void test_1000_large_short_sync(std::string ip, int port) {
  test_sync(1000, input_large, ip, port);
}

void test_1000_large_short_async(std::string ip, int port) {
  test_async(1000, input_small, ip, port);
}

void testLongConnection(SocketClient* client, int i) {
  for (int i = 0; i < 1000; i++) {
    logger_debug("---------");
    // testCallback(client, i);
    // testBigDataCallback(client, i);
  }
}