#pragma once

#include <iostream>
#include <mutex>

#include "core/Exception.hpp"

template <class T>
class Queue;

template <class T>
class QueueNode {
  friend class Queue<T>;

 private:
  T value;
  QueueNode* next;

  QueueNode(T value) : value(value), next(nullptr) {}
  ~QueueNode() { next == nullptr; }
};

template <class T>
class Queue {
 private:
  QueueNode<T>* header;
  QueueNode<T>* tail;
  size_t _size = 0;
  std::mutex mut;

 public:
  Queue();
  ~Queue();
  void push(T value);
  T front();
  void pop();
  bool empty() { return header == nullptr; }
  size_t size() { return _size; }
};

template <class T>
Queue<T>::Queue() : header(nullptr), tail(nullptr) {}

template <class T>
Queue<T>::~Queue() {
  while (!this->empty()) {
    pop();
  }
}

template <class T>
void Queue<T>::push(T value) {
  std::lock_guard lock(mut);

  if (header == nullptr) {
    tail = header = new QueueNode<T>(value);
  } else {
    tail->next = new QueueNode<T>(value);
    tail = tail->next;
  }
  _size++;
}

template <class T>
T Queue<T>::front() {
  if (header == nullptr) {
    throw Exception("queue is empty", ZONE);
  }
  return header->value;
}

template <class T>
void Queue<T>::pop() {
  if (header == nullptr) {
    throw Exception("Queue is empty", ZONE);
  }

  std::lock_guard lock(mut);
  auto cur = header;
  header = header->next;
  if (header == nullptr) {
    tail == nullptr;
  }

  delete cur;

  _size--;
}
