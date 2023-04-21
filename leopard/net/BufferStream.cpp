#include "BufferStream.hpp"

#include <atomic>
#include "log.hpp"

// std::atomic<int> streamsCount = 0;
BufferStream::BufferStream() {
  // streamsCount++;
  buffer = new char[capacity];
}

BufferStream::BufferStream(size_t len) {
  // streamsCount++;
  buffer = new char[len];
  _size = capacity = len;
  pos = 0;
}

BufferStream::~BufferStream() {
  if (buffer != nullptr) {
    delete[] buffer;
    buffer = nullptr;
    // streamsCount--;
    // printf("BufferStream dispose [%d] size:%d\n", streamsCount.load(), _size);
  }
}

BufferStream* BufferStream::pick() {
  // leopard_trace("< pick");

  size_t pickedLen = this->currentSize();
  auto picked = this->newInstance(pickedLen);
  memcpy(picked->buffer, this->buffer, pickedLen);

  size_t leftLen = this->_size - pickedLen;
  memcpy(this->buffer, this->buffer + pickedLen, leftLen);

  size_t newLen = leftLen > BUF_UNIT ? leftLen : BUF_UNIT;

  if (leftLen > BUF_UNIT) {
    this->realloc(newLen);
  }

  _size = pos = _size - picked->size();
  this->capacity = newLen;

  char end_str[8] = {'\0'};
  memcpy(end_str, picked->buffer + (picked->size() - 7), 7);
  end_str[7]='\0';

  leopard_trace("> pick,(%d)%s - end with:%s", pickedLen, picked->buffer + 32, end_str);

  return picked;
}

void BufferStream::realloc(int newSize) {
  Byte* newBuffer = new char[newSize];
  memcpy(newBuffer, buffer, _size);
  delete[] buffer;
  buffer = newBuffer;
}

void BufferStream::puts(Byte* buf, int len) {
  size_t t = pos + len;
  if (t > capacity) {
    // int old = capacity;
    capacity = BUF_UNIT * (t / BUF_UNIT) + BUF_UNIT;
    // buffer = realloc(buffer, capacity);
    this->realloc(capacity);

    // printf("BufferStream realloc %d -> %d\n", old, capacity);
  }
  memcpy(buffer + pos, buf, len);

  // int old = pos;
  pos += len;
  _size = pos;

  // printf("BufferStream pos %d -> %d len: %d\n", old, pos, len);
}

void BufferStream::moveToHead() {
  pos = 0;
}

void BufferStream::get(std::string& str, int len) {
  str.resize(len + 1);
  str[len] = '\0';
  this->get((Byte*)str.c_str(), len);
}
std::string BufferStream::getString(int len) {
  std::string str(len, '\0');
  this->get((Byte*)str.data(), len);
  return str;
}
void BufferStream::get(Byte* ptr, int len) {
  memcpy(ptr, buffer + pos, len);
  pos += len;
}

std::string BufferStream::toHex() {
  std::stringstream out;
  char* str = (char*)buffer;
  for (int i = 0; i < _size; i++) {
    out << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << (int)str[i];
  }
  out << "  ";

  return out.str();
}