#include "Antelope.hpp"

#include <signal.h>

#include "Lane.hpp"
#include "core/Exception.hpp"

#define BUFFER_SIZE 1024

Antelope Antelope::instance;

void Antelope::newInstance(int id, bool* running) {
  auto runway = new Lane(id, running);
  this->runways.push_back(runway);
}

Connection* Antelope::create(const char* ip, int port) {
  auto runway = std::min_element(runways.begin(), runways.end(), [](Lane* x, Lane* y) { return x->size() - y->size(); });
  if (runway == runways.end()) {
    throw Exception("ERROR", ZONE);
  }

  Connection* skt = (*runway)->create(ip, port);
  return skt;
}

void Antelope::send(Connection* connection, BufferStreamPtr outputStream) {
  auto runway = std::min_element(runways.begin(), runways.end(), [](Lane* x, Lane* y) { return x->size() - y->size(); });
  if (runway == runways.end()) {
    throw Exception("ERROR", ZONE);
  }

  (*runway)->send(connection, outputStream);
}