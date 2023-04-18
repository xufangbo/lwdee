#include "BufferStream.hpp"

BufferStream::BufferStream() {
  buffer = (Byte*)malloc(capacity);
}

BufferStream::~BufferStream() {
  if (buffer != NULL) {
    free(buffer);
    buffer = NULL;
  }
}

void BufferStream::moveToHead() {
  pos = 0;
}

void BufferStream::next() {
  size_t currentLen = this->currentSize();

  this->pos = this->_size - currentLen;
  this->_size = this->pos;
  memcpy(buffer + currentLen, buffer, this->_size);
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
  for (int i = 0; i < size(); i++) {
    out << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
        << (int)str[i];
  }
  out << "  ";

  return out.str();
}