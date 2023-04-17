#include "Antelope.hpp"

#include <signal.h>

#include "core/Exception.hpp"
#include "core/Stopwatch.hpp"
#include "core/log.hpp"
#include "net/LeopardConfig.hpp"
#include "net/LeopardProtocal.hpp"
#include "net/ProtocalFactory.hpp"

#define BUFFER_SIZE 1024

Antelope Antelope::instance;

Antelope::~Antelope() {
  for (int i = 0; i < lanes.size(); i++) {
    delete lanes[i];
  }
  lanes.clear();
}

void Antelope::start() {
  signal(SIGPIPE, SIG_IGN);
  if (running) {
    return;
  }

  sendQueue.start();
  for (int i = 0; i < 1; i++) {
    lanes.push_back(new Lane());
  }

  running = true;
}

SocketClientPtr Antelope::create(const char* ip, int port) {
}

void Antelope::tpsJob() {
  while (running) {
    sleep(1);
  }
}