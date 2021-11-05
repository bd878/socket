#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int
main(int argc, char **argv) {
  printf("Executing main process\n");
  pid_t p = fork();
  if (p == -1) {
    perror("fork");
    exit(1);
  }

  if (p == 0) {
    printf("in child process before execed\n");
    execl("./execed.out", "127.0.0.1\0", NULL);
    exit(0);
  }

  printf("child proc id: %d\n", p);
  printf("stop main process\n");

  return 0;
}