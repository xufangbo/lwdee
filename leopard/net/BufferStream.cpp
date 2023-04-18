#include "BufferStream.hpp"

#include "log.hpp"

BufferStream::BufferStream() {
  buffer = (Byte*)malloc(capacity);
}

BufferStream::BufferStream(size_t len) {
  buffer = (Byte*)malloc(len);
  _size = capacity = len;
  pos = 0;
}

BufferStream::~BufferStream() {
  if (buffer != NULL) {
    free(buffer);
    buffer = NULL;
  }
}

BufferStreamPtr BufferStream::pick() {
  leopard_trace("< pick");

  size_t pickedLen = this->currentSize();
  auto picked = this->newInstance(pickedLen);
  picked->puts(this->buffer, pickedLen);

  size_t leftLen = this->_size - pickedLen;
  Byte* left = (Byte*)malloc(leftLen);
  memcpy(left, buffer + pickedLen, leftLen);

  size_t newLen = leftLen > BUF_UNIT ? leftLen : BUF_UNIT;
  buffer = realloc(buffer, newLen);
  this->pos = 0;
  this->_size = 0;
  this->capacity = newLen;

  this->puts(left, leftLen);

  free(left);

  leopard_trace("> pick,(%d)%s - (%d)%s", pickedLen, picked->buffer, _size, buffer);

  return picked;
}

void BufferStream::puts(Byte* buf, int len) {
  size_t t = pos + len;
  if (t > capacity) {
    // int old = capacity;
    capacity = BUF_UNIT * (t / BUF_UNIT) + BUF_UNIT;
    buffer = realloc(buffer, capacity);

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