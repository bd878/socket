#include "lib.h"
#include <netinet/tcp.h>

int
main(int argc, char **argv) {
  static const int MAXLINE = 1024;

  struct sockaddr_in servaddr;
  int fd, n, rcvbuf, rcvbufafter, mss, mssafter, intlen;
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

  intlen = sizeof(rcvbuf);
  Getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &rcvbuf, &intlen);
  Getsockopt(fd, IPPROTO_TCP, TCP_MAXSEG, &mss, &intlen);
  printf("SO_RCVBUF before = %d\n", rcvbuf);
  printf("MSS before = %d\n", mss);

  printf("...connecting\n");
  Connect(fd, (struct sockaddr *)&servaddr, sizeof(servaddr));

  Getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &rcvbufafter, &intlen);
  Getsockopt(fd, IPPROTO_TCP, TCP_MAXSEG, &mssafter, &intlen);
  printf("SO_RCVBUF after = %d\n", rcvbufafter);
  printf("MSS before = %d\n", mssafter);

  while ((n = Read(fd, recvline, MAXLINE)) > 0) {
    recvline[n] = 0;
    Fputs(recvline, stdout);
  }

  return 0;
}