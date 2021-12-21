#include "lib.h"

int
main(int argc, char **argv) {
  static const int MAXLINE = 1024;

  struct sockaddr_in servaddr;
  int fd, n;
  char recvline[MAXLINE + 1];

  if (argc < 2) {
    perror("usage: daytimeclient.out <IPAddress>");
    exit(1);
  }

  fd = Socket(AF_INET, SOCK_STREAM, 0);
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(5436);
  Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

  printf("...connecting\n");

  Connect(fd, (struct sockaddr *)&servaddr, sizeof(servaddr));
  while ((n = Read(fd, recvline, MAXLINE)) > 0) {
    recvline[n] = 0;
    Fputs(recvline, stdout);
  }

  return 0;
}