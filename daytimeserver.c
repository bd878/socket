#include "lib.h"
#include <time.h>

int
main() {
  static const int MAXLINE = 1024;

  int listenfd, connfd;
  struct sockaddr_in addr;
  char sendline[MAXLINE];
  time_t ts;

  listenfd = Socket(AF_INET, SOCK_STREAM, 0);
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(5436);
  addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

  Bind(listenfd, (struct sockaddr *)&addr, sizeof(addr));

  Listen(listenfd, 1);

  while (1) {
    connfd = Accept(listenfd, NULL, NULL);
    printf("connection received\n");

    ts = time(NULL);
    snprintf(sendline, sizeof(sendline), "%.24s\er\en", ctime(&ts));
    Write(connfd, sendline, strlen(sendline));

    Close(connfd);
  }

  return 0;
}