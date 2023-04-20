#include "LaneClients.hpp"
#include "net/log.hpp"


LaneClientPtr LaneClients::create(const char* ip, int port) {
  auto client = LaneClient::create(ip, port);
  this->clients.push_back(client);
  return client;
}

void LaneClients::wait() {
  for (int i = 0; i < clients.size(); i++) {
    auto client = this->clients[i];
    try {
      // printf("\nwait %d\n\n", i);
      client->wait();
    } catch (Exception& ex) {
      logger_error("[%d] - %s", (i + 1), ex.getMessage().c_str());
    } catch (std::exception& ex) {
      logger_error("[%d] - %s", (i + 1), ex.what());
    }
  }
}

void LaneClients::close() {
  for (int i = 0; i < clients.size(); i++) {
    auto client = this->clients[i];
    try {
      client->close();
    } catch (Exception& ex) {
      logger_error("[%d] - %s", (i + 1), ex.getMessage().c_str());
    } catch (std::exception& ex) {
      logger_error("[%d] - %s", (i + 1), ex.what());
    }
  }
}