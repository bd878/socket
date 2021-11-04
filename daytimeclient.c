#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int BUF_LEN = 1000;

// 129.6.15.28
// 128.138.140.44
int
main(int argc, char **argv) {
  int sock, n;
  char recvline[BUF_LEN + 1];
  struct sockaddr_in serveaddr;

  if (argc != 2) {
    perror("usage: a.out <IPaddress>");
    exit(1);
  }

  // if ((sock = socket(9999, SOCK_STREAM, 0)) < 0) {
  //   perror("sock");
  //   exit(1);
  // }

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    exit(1);
  }

  memset(&serveaddr, 0, sizeof(serveaddr));
  serveaddr.sin_family = AF_INET;
  serveaddr.sin_port = htons(13);
  // serveaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  if (inet_pton(AF_INET, argv[1], &serveaddr.sin_addr) <= 0) {
    printf("inet_pton error for %s\n", argv[1]);
    exit(1);
  }

  printf("connecting...\n");
  if (connect(sock, (struct sockaddr*)&serveaddr, sizeof(serveaddr)) > 0) {
    // ECONNREFUSED, EINTR, ETIMEDOUT, rst handshake packet
    perror("connect");
    exit(2);
  }
  printf("connected\n");

  // n = recv(sock, recvline, sizeof(BUF_LEN), 0);
  // recvline[n] = 0;
  // if (fputs(recvline, stdout) == EOF) {
  //   perror("fputs error");
  // }
  int success_read_count = 0;
  while ((n = read(sock, recvline, BUF_LEN)) > 0) {
    success_read_count += 1;

    recvline[n] = 0;
    if (fputs(recvline, stdout) == EOF) {
      printf("%s\n", strerror(errno));
      exit(2);
    }
  }

  printf("\n");
  printf("Successive reads %d\n", success_read_count);

  if (n < 0) {
    perror("read");
    exit(3);
  }

  return 0;
}