#include "Antelope.hpp"

#include <signal.h>

#include "Lane.hpp"

#define BUFFER_SIZE 1024

Antelope Antelope::instance;

Antelope::~Antelope() {
  for (int i = 0; i < runways.size(); i++) {
    delete runways[i];
  }
  runways.clear();
}

void Antelope::start() {
  signal(SIGPIPE, SIG_IGN);
  if (running) {
    return;
  }

  sendQueue.start();
  for (int i = 0; i < 1; i++) {
    auto lane = new Lane(i + 1, &running, &sendQueue);
    runways.push_back(lane);
  }

  running = true;
}

SocketClientPtr Antelope::create(const char* ip, int port) {
  return nullptr;
}