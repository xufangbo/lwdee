#include "ClientWaitor.hpp"

#include "ClientConnection.hpp"
#include "LaneClients.hpp"
#include "log.hpp"

void ClientWaitor_t::notify(WaitStatus status) {
  this->status = status;
  if (status == WaitStatus::succeed) {
    this->client->onInvoked(connection);
  }
}

float ClientWaitor_t::wait(float timeout) {
  while (sw.elapsed() < timeout) {
    if (status == WaitStatus::succeed) {
      return sw.stop();
    } else if (status == WaitStatus::timeout) {
      sw.stop();
      throw Exception("timeout", ZONE);
    } else if (status == WaitStatus::nohint) {
      return sw.stop();
      throw Exception("nohint", ZONE);
    } else {
      usleep(1000 * 100);
    }
  }
  leopard_error("[%d] timeout in %lfs", id, timeout);
  return timeout;
}