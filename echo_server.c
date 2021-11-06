#include "lib.h"

void
str_echo(int fd) {
  static const int MAXLINE = 1024;
  ssize_t n;
  char buf[MAXLINE];

  while (1) {
    if ((n = read(fd, buf, MAXLINE)) == 0) {
      return;
    }

    Writen(fd, buf, n);
  }
}

int
main(int argc, char **argv) {
  int listenfd, cfd;
  pid_t childpid;
  struct sockaddr_in caddr, saddr;
  socklen_t clen;

  listenfd = Socket(AF_INET, SOCK_STREAM, 0);

  memset(&saddr, 0, sizeof(saddr));
  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = htonl(INADDR_ANY);
  saddr.sin_port = htons(9877);

  Listen(listenfd, 5);

  while (1) {
    clen = sizeof(caddr);
    cfd = Accept(listenfd, (struct sockaddr *)&caddr, &clen);

    if ((childpid = Fork()) == 0) {
      Close(listenfd);
      str_echo(cfd);
      exit(0);
    }
    Close(cfd);
  }
}