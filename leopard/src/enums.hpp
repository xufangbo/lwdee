#pragma once

#include <iostream>

enum class ApplicationType : uint8_t {
  server = 1,
  client = 2
};

enum class SendSource : uint8_t {
  none = 0,
  request = 1,
  epoll_out = 2
};

enum class CloseType : uint8_t {
  normal = 1,
  err_recv = 2,
  err_send = 3,
  no_router = 4,
  unknown = 5,
};