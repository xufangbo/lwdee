#pragma once

#include "IRunway.hpp"

template <class T>
class IRunwayContainer {
 protected:
  std::vector<T*> runways;
  SendTaskQueue sendQueue;
  bool running;

 protected:
  void qpsJob();
};