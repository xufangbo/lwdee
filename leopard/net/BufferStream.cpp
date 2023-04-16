#include "BufferStream.hpp"

BufferStream::BufferStream() {
  buffer = (Byte*)calloc(1, capacity);
}

BufferStream::~BufferStream() {
  if (buffer != NULL) {
    free(buffer);
    buffer = NULL;
  }
}

void BufferStream::reset() {
  pos = 0;
  _size = 0;
}

void BufferStream::puts(Byte* buf, int len) {
  size_t t = pos + len;
  if (t > capacity) {
    capacity = BUF_UNIT * (t / BUF_UNIT) + BUF_UNIT;
    buffer = realloc(buffer, capacity);
  }
  memcpy(buffer + pos, buf, len);
  pos += len;
  _size = pos;
}

void BufferStream::get(std::string& str, int len) {
  str.resize(len + 1);
  str[len] = '\0';
  this->get((Byte*)str.c_str(), len);
}
std::string BufferStream::getString(int len) {
  std::string str;
  this->get(str, len);
  return str;
}
void BufferStream::get(Byte* ptr, int len) {
  memcpy(ptr, buffer + pos, len);
  pos += len;
}

void BufferStream::clean() {
  memset(buffer, pos, '\0');
  pos = 0;
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