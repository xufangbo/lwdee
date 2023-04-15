#pragma once

#include <chrono>
#include <coroutine>
#include <functional>
#include <iostream>
#include <memory>
#include <thread>

struct suspend {
  struct promise_type {
    promise_type() {}
    suspend get_return_object() {
      return {std::coroutine_handle<suspend::promise_type>::from_promise(*this)};
    }
    std::suspend_never initial_suspend() { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() {}
  };

  typedef std::coroutine_handle<suspend::promise_type> SuspendHandler;

  SuspendHandler handle_;  // 这个变量名可以随便起
};

template <class T>
struct await {

  typedef std::function<void(suspend::SuspendHandler, T*)> AwaiteFunction;

  AwaiteFunction fun;
  T returnValue;

  await(AwaiteFunction fun) : fun(fun){};
  bool await_ready() { return false; }
  void await_suspend(suspend::SuspendHandler handle) {
    T *ptr = &returnValue;
    fun(handle, ptr);
  }
  T await_resume() { return returnValue; }
};