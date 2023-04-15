#include "TcpRequest.hpp"

#include <algorithm>

std::map<const std::string, RequestCallback> TcpRequest::requestCallbacks = {};


void TcpRequest::regist(const std::string &path,
                        RequestCallback responseFunction) {
  requestCallbacks.insert(std::make_pair(path, responseFunction));
}

RequestCallback *TcpRequest::find(const std::string path) {
  for (auto &i : requestCallbacks) {
    if (strcmp(path.c_str(), i.first.c_str()) == 0) {
      return &i.second;
    }
  }

  return nullptr;
}

//=====================

std::map<const std::string, SuspendCallback> TcpRequest::suspendCallbacks = {};

void TcpRequest::registSuspend(const std::string &path,
                               SuspendCallback suspendCallback) {
  suspendCallbacks.insert(std::make_pair(path, suspendCallback));
}

SuspendCallback *TcpRequest::findSuspend(const std::string path) {
  for (auto &i : suspendCallbacks) {
    if (strcmp(path.c_str(), i.first.c_str()) == 0) {
      return &i.second;
    }
  }

  return nullptr;
}