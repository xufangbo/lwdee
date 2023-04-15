#pragma once

#include <functional>
#include <iostream>
#include <map>

#include "net/BufferStream.hpp"

typedef std::function<void(BufferStream *inputStream,BufferStream *outputStream)> ResponseFunction;

class TcpResponse {
 private:
  static std::map<const std::string, ResponseFunction> responseFunctions;

 public:
  static void regist(const std::string &path, ResponseFunction responseFunction);
  static ResponseFunction * find(const std::string path);
};