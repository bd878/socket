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

    // printf("received: %s\n", buf);

    Writen(fd, buf, n);
  }
}

void
sig_chld(int signo) {
  pid_t pid;
  int stat;

  pid = wait(&stat);
  printf("child terminated %d\n", pid);
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

  Bind(listenfd, (struct sockaddr *)&saddr, sizeof(saddr));
  Listen(listenfd, 5);
  Signal(SIGCHLD, sig_chld);

  // printf("listening on %d port\n", ntohs(saddr.sin_port));

  while (1) {
    clen = sizeof(caddr);
    // printf("waiting for connections...\n");
    cfd = Accept(listenfd, (struct sockaddr *)&caddr, &clen);
    // printf("connection received on %d port\n", ntohs(caddr.sin_port));

    if ((childpid = Fork()) == 0) {
      Close(listenfd);
      str_echo(cfd);
      exit(0);
    }
    Close(cfd);
  }
}