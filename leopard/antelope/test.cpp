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

void test_1000_small_short_sync(std::string ip, int port) {
  Stopwatch sw;
  for (int i = 0; i < 1000; i++) {
    try {
      auto client = SocketClient::create(ip.c_str(), port, 1);

      std::string input = "green green green " + std::to_string(i);
      logger_debug("send %s", input.c_str());

      RequestCallback callback = [i](BufferStream* inputStream) {
        responseIndex++;
        auto len = inputStream->get<uint32_t>();
        auto content = inputStream->getString(len);

        logger_debug("recive-%d:  (%d)%s", responseIndex, len, content.c_str());
      };

      SocketWaiter waiter = client->invoke("com.cs.sales.order.save", (void*)input.c_str(), input.size(), callback);

      auto time = waiter->wait();
      logger_info("wait %d eclipse %.3lfs ---------------------------------------", i, time);

      client->close();

    } catch (Exception& ex) {
      logger_warn("%s", ex.getMessage().c_str());
    } catch (std::exception& ex) {
      logger_error("%s", ex.what());
    }
  }

  logger_info("test_1000_small_short ,elapsed %.3lf", sw.stop());
}

void test_1000_small_short_async(std::string ip, int port) {
  Stopwatch sw;

  std::vector<SocketWaiter> waiters;
  std::vector<SocketClientPtr> clients;
  for (int i = 0; i < 1000; i++) {
    try {
      auto client = SocketClient::create(ip.c_str(), port, 1);

      std::string input = "green green green " + std::to_string(i);
      logger_debug("send %s", input.c_str());

      RequestCallback callback = [i](BufferStream* inputStream) {
        responseIndex++;
        auto len = inputStream->get<uint32_t>();
        auto content = inputStream->getString(len);

        logger_debug("recive-%d:  (%d)%s", responseIndex, len, content.c_str());
      };

      SocketWaiter waiter = client->invoke("com.cs.sales.order.save", (void*)input.c_str(), input.size(), callback);

      waiters.push_back(waiter);
      clients.push_back(client);

    } catch (Exception& ex) {
      logger_warn("%s", ex.getMessage().c_str());
    } catch (std::exception& ex) {
      logger_error("%s", ex.what());
    }
  }

  for (int i = 0; i < waiters.size(); i++) {
    try {
      waiters[i]->wait();
      clients[i]->close();
    } catch (Exception& ex) {
      logger_warn("[%d] %s", i, ex.getMessage().c_str());
    } catch (std::exception& ex) {
      logger_error("[%d] %s", i, ex.what());
    }
  }

  logger_info("test_1000_small_short ,elapsed %.3lf", sw.stop());
}

void testLongConnection(SocketClient* client, int i) {
  for (int i = 0; i < 1000; i++) {
    logger_debug("---------");
    // testCallback(client, i);
    testBigDataCallback(client, i);
  }
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