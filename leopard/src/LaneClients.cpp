#include "LaneClients.hpp"
#include "src/log.hpp"


LaneClientPtr LaneClients::create(const char* ip, int port) {
  auto client = LaneClient::create(ip, port);
  this->clients.push_back(client);
  return client;
}

void LaneClients::wait(float timeout) {
  for (int i = 0; i < clients.size(); i++) {
    auto client = this->clients[i];
    try {
      // printf("\nwait %d\n\n", i);
      client->wait(timeout);
    } catch (Exception& ex) {
      logger_error("[%d] - %s", (i + 1), ex.getMessage().c_str());
    } catch (std::exception& ex) {
      logger_error("[%d] - %s,%s:%d",(i + 1), ex.what(),__FILE__,__LINE__);
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
      logger_error("[%d] - %s,%s:%d",(i + 1), ex.what(),__FILE__,__LINE__);
    }
  }
}