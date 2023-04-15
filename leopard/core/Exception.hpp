#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define ZONE __FUNCTION__, __FILE__, __LINE__

/**
 * 异常基类
 **/
class Exception {
 protected:
  std::string function;
  std::string file;
  int line;
  std::string message;
  std::vector<std::string> traces;

 public:
  Exception(std::string message, const char* function, const char* file, int line);

  virtual std::string getMessage();
  virtual std::string name() { return "Exception"; };
  std::string getStackTrace();

  void trace(const char* function, const char* file, int line);
  void traceMessage(std::string message, const char* function, const char* file, int line);
};

/**
 * 文件异常
 **/
class NullPointerException : public Exception {
 public:
  NullPointerException(const char* function, const char* file, int line)
      : Exception("null pointer exception", function, file, line){};
  std::string name() override { return "NullPointerException"; };
};

/**
 * 文件异常
 **/
class LwdeeException : public Exception {
 public:
  LwdeeException(const char* function, const char* file, int line)
      : Exception("null pointer exception", function, file, line){};
  LwdeeException(std::string message, const char* function, const char* file, int line)
      : Exception(message, function, file, line){};
  std::string name() override { return "LwdeeException"; };
};

/**
 * Json异常
 **/
class JsonException : public Exception {
 public:
  JsonException(std::string message, const char* function, const char* file, int line)
      : Exception(message, function, file, line){};
  std::string name() override { return "JsonException"; };
};

/**
 * Reflect异常
 **/
class ReflectException : public Exception {
 public:
  ReflectException(std::string message, const char* function, const char* file, int line)
      : Exception(message, function, file, line){};
  std::string name() override { return "ReflectException"; };
};

/**
 * 未实现异常
 **/
class NotImplementException : public Exception {
 public:
  NotImplementException(const char* function, const char* file, int line)
      : Exception("not implement exception", function, file, line){};
  std::string name() override { return "NotImplementException"; };
};

/**
 * 文件异常
 **/
class FileException : public Exception {
 public:
  FileException(std::string message, const char* function, const char* file, int line)
      : Exception(message, function, file, line){};
  std::string name() override { return "FileException"; };
};

/**
 * 线程异常
 **/
class ThreadException : public Exception {
 public:
  ThreadException(std::string message, const char* function, const char* file, int line)
      : Exception(message, function, file, line){};
  std::string name() override { return "ThreadException"; };
};

/**
 * 数据访问异常
 **/
class DataAccessException : public Exception {
 public:
  DataAccessException(std::string message, const char* function, const char* file, int line)
      : Exception(message, function, file, line){};
  std::string name() override { return "DataAccessException"; };
};

/**
 * HttpException
 **/
class HttpException : public Exception {
 public:
  HttpException(std::string message, const char* function, const char* file, int line)
      : Exception(message, function, file, line){};
  std::string name() override { return "HttpException"; };
};

/**
 * socket异常
 **/
class SocketException : public Exception {
 public:
  int errorCode;
  SocketException(int errorCode, const char* function, const char* file, int line)
      : Exception("", function, file, line) {
    this->errorCode = errorCode;
    this->message = this->getError(errorCode);
  }
  SocketException(std::string msg, int errorCode, const char* function, const char* file, int line)
      : Exception("", function, file, line) {
    this->errorCode = errorCode;
    this->message = msg + ",(" + std::to_string(errorCode) + ")" + this->getError(errorCode);
  }
  std::string name() override { return "SocketException"; };

 private:
  std::string getError(int errorCode);
};

class EpollException : public Exception {
 public:
  int errorCode;
  EpollException(int errorCode, const char* function, const char* file, int line)
      : Exception("", function, file, line) {
    this->errorCode = errorCode;
    this->message = this->getError(errorCode);
  }
  EpollException(std::string msg, int errorCode, const char* function, const char* file, int line)
      : Exception("", function, file, line) {
    this->errorCode = errorCode;
    this->message = msg + "," + this->getError(errorCode);
  }
  std::string name() override { return "EpollException"; };

 private:
  std::string getError(int errorCode);
};