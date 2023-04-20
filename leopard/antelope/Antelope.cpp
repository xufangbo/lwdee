#include "Antelope.hpp"

#include <signal.h>

#include "Lane.hpp"
#include "core/Exception.hpp"

#define BUFFER_SIZE 1024

Antelope Antelope::instance;

void Antelope::newInstance(int id, bool* running, SendTaskQueue* sendQueue) {
  auto runway = new Lane(id, running, sendQueue);
  this->runways.push_back(runway);
}

ClientSocket* Antelope::create(const char* ip, int port) {
  auto runway = std::min_element(runways.begin(), runways.end(), [](Lane* x, Lane* y) { return x->size() - y->size(); });
  if (runway == runways.end()) {
    throw Exception("ERROR", ZONE);
  }

  ClientSocket* skt = (*runway)->create(ip, port);
  return skt;
}

void Antelope::send(Socket* socket, BufferStreamPtr outputStream) {
  auto runway = std::min_element(runways.begin(), runways.end(), [](Lane* x, Lane* y) { return x->size() - y->size(); });
  if (runway == runways.end()) {
    throw Exception("ERROR", ZONE);
  }

  (*runway)->send(socket, outputStream);
}