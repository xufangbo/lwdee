#include "Antelope.hpp"

#include <signal.h>

#include "Lane.hpp"
#include "core/Exception.hpp"

#define BUFFER_SIZE 1024

Antelope Antelope::instance;

void Antelope::start() {
  signal(SIGPIPE, SIG_IGN);
  if (running) {
    return;
  }

  sendQueue.start(&running);
  for (int i = 0; i < 1; i++) {
    auto lane = new Lane(i + 1, &running, &sendQueue);
    lane->start();
    
    runways.push_back(lane);
  }

  running = true;
}

Socket* Antelope::create(const char* ip, int port) {
  auto it = std::min_element(runways.begin(), runways.end(), [](Lane* x, Lane* y) { return x->size() - y->size(); });
  if (it == runways.end()) {
    throw Exception("ERROR", ZONE);
  }

  Socket* skt = (*it)->create(ip, port);
  return skt;
}

void Antelope::send(Socket* socket, BufferStreamPtr outputStream) {
  auto it = std::min_element(runways.begin(), runways.end(), [](Lane* x, Lane* y) { return x->size() - y->size(); });
  if (it == runways.end()) {
    throw Exception("ERROR", ZONE);
  }

  (*it)->send(socket, outputStream);
}

bool Antelope::contains(int fd) {
  for (auto it : runways) {
    if (it->contains(fd)) {
      return true;
    }
  }
  return false;
}