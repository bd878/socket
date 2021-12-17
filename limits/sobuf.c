#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>

int
main() {
  int fd, val, len;
  fd = socket(AF_INET, SOCK_STREAM, 0);

  len = sizeof(val);
  if (getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &val, &len) == -1) {
    perror("getsockopt");
  } else {
    printf("default SO_SNDBUF = %d\n", val);
  }

  exit(1);
}
