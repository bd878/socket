#include "../lib.h"

int main() {
  static const int MAXLINE = 1024;
  struct sockaddr_in addr, cliaddr;
  int listenfd, clifd, dnready;
  int clients[FD_SETSIZE];
  int maxfd, maxi;
  ssize_t nread;
  socklen_t clilen;
  char readbuf[MAXLINE];
  fd_set allset, rset;

  listenfd = Socket(AF_INET, SOCK_STREAM, 0);
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(5436);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  Bind(listenfd, (struct sockaddr *)&addr, sizeof(addr));
  Listen(listenfd, 1);

  for (int i = 0; i < FD_SETSIZE; ++i) {
    clients[i] = -1;
  }

  maxi = 0;
  maxfd = listenfd;

  FD_ZERO(&allset);
  FD_ZERO(&rset);

  FD_SET(listenfd, &allset);

  while (1) {
    rset = allset;
    dnready = Select(maxfd + 1, &rset, NULL, NULL, NULL);

    if (FD_ISSET(listenfd, &rset)) {
      clilen = sizeof(cliaddr);
      clifd = Accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);

      for (int i = 0, isplaced = 0; i < FD_SETSIZE && isplaced == 0; ++i) {
        if (clients[i] < 0) {
          clients[i] = clifd;
          isplaced = 1;
          if (i == maxi) {
            maxi += 1;
          }
        }
      }
      /* if (i == FD_SETSIZE) { perror("too many clients"); } */
      FD_SET(clifd, &allset);
      maxfd = max(maxfd, clifd);
      dnready -= 1;
    }

    for (int i = 0; i <= maxi && dnready > 0; ++i) {
      if (clients[i] > 0) {
        if (FD_ISSET(clients[i], &rset)) {
          if ((nread = Read(clients[i], readbuf, MAXLINE)) == 0) {
            Close(clients[i]);
            FD_CLR(clients[i], &allset);
            clients[i] = -1;
          } else {
            Writen(clients[i], readbuf, nread);
            memset(&readbuf, 0, MAXLINE);
          }
        }

        dnready -= 1;
      }
    }
  }
}