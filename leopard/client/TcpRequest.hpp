#pragma once

#include <functional>
#include <iostream>
#include <map>

#include "core/suspend.hpp"
#include "net/BufferStream.hpp"

typedef std::function<void(BufferStream*)> RequestInvoke;
typedef std::function<void(BufferStream*)> RequestCallback;

struct SuspendCallback {
  suspend::SuspendHandler handle;
  BufferStream** returnValue;
  void callback(BufferStream* inputStream) {
    *returnValue = inputStream;
    handle.resume();
  }
};

class TcpRequest {
 private:
 /**
  * 这里的key应该为messageId
 */
  static std::map<const std::string, RequestCallback> requestCallbacks;
  static std::map<const std::string, SuspendCallback> suspendCallbacks;

 public:
  static void regist(const std::string& path, RequestCallback responseFunction);
  static RequestCallback* find(const std::string path);

  static void registSuspend(const std::string& path, SuspendCallback suspendCallback);
  static SuspendCallback* findSuspend(const std::string path);
};