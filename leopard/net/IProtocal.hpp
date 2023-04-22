#pragma once

#include "BufferStream.hpp"
#include "Connection.hpp"
#include "IRunway.hpp"
#include "ClientSocket.hpp"

typedef std::function<void(BufferStream*)> RequestInvoke;
typedef std::function<void(BufferStream*)> RequestCallback;

class IProtocal {
 public:
 /**
  * @brief 生成请求消息，并初始化结构信息，这样业务可以基于头后面的偏移量进行写入
  * 
  * @param request 
  * @param path 
  * @return BufferStream* 
  */
  virtual BufferStream* newStream(std::string& path) = 0;

  /**
   * @brief 发送一个请求
   * 
   */
  virtual SocketWaiter csend(IRunway* runway, Connection* connection, BufferStream* outputStream) = 0;

  /**
   * @brief 客户端接收请求
   * 
   * @param runway 
   * @param connection 
   * @param inputStream 
   */
  virtual void caccept(IRunway* runway, Connection* connection, BufferStream* inputStream) = 0;

  /**
   * @brief 服务端接收请求
   * 
   * @param runway 
   * @param connection 
   * @param inputStream 
   */
  virtual void saccept(IRunway* runway, Connection* connection, BufferStream* inputStream) = 0;
};

typedef std::shared_ptr<IProtocal> LeopardProtocalPtr;