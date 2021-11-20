#include <stdio.h>
#include <string.h>

int main() {
  static int MAX_SIZE = 1024;
  char arr[MAX_SIZE];
  printf("sizeof(arr) = %ld, strlen(arr) = %ld\n", sizeof(arr), strlen(arr));
  return 0;
}