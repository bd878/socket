#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

void
main() {
  struct rlimit limit;
  if (getrlimit(RLIMIT_NOFILE, &limit) < 0) {
    perror("getrlimit");
    exit(1);
  }
  printf("fds limits: ");
  printf("soft = %ld; hard = %ld\n",
    limit.rlim_cur, limit.rlim_max);

  limit.rlim_cur = limit.rlim_max;
  if (setrlimit(RLIMIT_NOFILE, &limit) < 0) {
    perror("setrlimit");
    exit(1);
  }

  printf("soft limit raised to max limit\n");

  printf("fds limits: ");
  printf("soft = %ld; hard = %ld\n",
    limit.rlim_cur, limit.rlim_max);
}