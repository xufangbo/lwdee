#pragma once

#include <functional>
#include <iostream>
#include <map>

#include "core/suspend.hpp"
#include "net/BufferStream.hpp"
#include "net/IProtocal.hpp"

#ifdef LEOPARD_SUSPEND
struct SuspendCallback {
  suspend::SuspendHandler handle;
  BufferStream** returnValue;
  void callback(BufferStream* inputStream) {
    *returnValue = inputStream;
    handle.resume();
  }
};
#endif

class TcpRequest {
 private:
  /**
   * 这里的key应该为messageId
   */
  static std::map<const std::string, RequestCallback> requestCallbacks;

 public:
  static void regist(const std::string& path, RequestCallback responseFunction);
  static RequestCallback* find(const std::string path);

#ifdef LEOPARD_SUSPEND
 private:
  static std::map<const std::string, SuspendCallback> suspendCallbacks;

 public:
  static void registSuspend(const std::string& path, SuspendCallback suspendCallback);
  static SuspendCallback* findSuspend(const std::string path);
#endif
};