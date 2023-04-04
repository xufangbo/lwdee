#include <memory.h>
#include <iostream>

int cmpfunc(const void* a, const void* b) {
  uint32_t left;
  ::memcpy(&left, a, 4);

  uint32_t right;
  ::memcpy(&right, b, 4);

  if (left > right) {
    return 1;
  } else {
    return left < right ? -1 : 0;
  }
}

int main() {
  int ar[] = {45, 2, 63, 41, 16, 75};
  ::qsort(ar, sizeof(ar) / sizeof(int), sizeof(int), cmpfunc);

  for (int& i : ar) {
    std::cout << i << " ";
  }
  std::cout << std::endl;
}