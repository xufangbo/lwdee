#include "TcpResponse.hpp"

#include <algorithm>

std::map<const std::string, ResponseFunction> TcpResponse::responseFunctions = {};

void TcpResponse::regist(const std::string& path, ResponseFunction responseFunction) {
  responseFunctions.insert(std::make_pair(path, responseFunction));
}

ResponseFunction* TcpResponse::find(const std::string path) {
  
  for (auto& i : responseFunctions) {
    if (strcmp(path.c_str(), i.first.c_str()) == 0) {
      return &i.second;
    }
  }

  return nullptr;
}