#include <sys/types.h>
#include <cstdlib>
#include <iostream>

using namespace std;

int main() {
  void *n_addr;
  char arr[17];
  for (int i = 0; i < sizeof(arr); ++i) {
    printf("%02x\n", ((uint8_t *)arr)[i]);
  }
  return 0;
}