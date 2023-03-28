#include <memory.h>
#include <iostream>

int main() {
  unsigned long long l = 7;
  printf("%016llx\n", l);

  //   FILE* fp = fopen("./long.bin", "w");
  //   fwrite(&l, 8, 1, fp);
  //   fclose(fp);
}