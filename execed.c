#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char **argv) {
  if (argc < 1) {
    printf("Usage: a.out <ip>\n");
    exit(1);
  }

  char *ip = argv[0];
  printf("ip string received %s\n", ip);

  return 0;
}