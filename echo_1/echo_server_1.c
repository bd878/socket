#include "../lib.h"

void
str_echo(int fd) {
  static const int MAXLINE = 1024;
  char line[MAXLINE];
  ssize_t n;

  while (1) {
    if ((n = read(fd, line, MAXLINE)) == 0) {
      return;
    }

    Writen(fd, line, n);
  }
}

int main() {
  struct sockaddr_in addr, caddr;
  int sockfd, cfd;
  pid_t childpid;
  socklen_t clen;

  sockfd = Socket(AF_INET, SOCK_STREAM, 0);

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(5436);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  Bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
  Listen(sockfd, 5);

  while (1) {
    clen = sizeof(caddr);
    cfd = Accept(sockfd, (struct sockaddr *)&caddr, &clen);

    if ((childpid = Fork()) == 0) {
      Close(sockfd);
      str_echo(cfd);
      exit(0);
    }

    Close(cfd);
  }

  exit(0);
}