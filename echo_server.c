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

/* ps -t pts/N -o pid,ppid,tty,stat,args,wchan */
void
sig_chld(int signo) {
  pid_t pid;
  int stat;

  while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
    printf("child terminated %d\n", pid);
  }
  return;
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
    if ((cfd = accept(listenfd, (struct sockaddr *)&caddr, &clen)) < 0) {
      if (errno = EINTR) {
        continue; /* slow accept(...) interrupted by sys call */
      } else {
        perror("accept");
        exit(1);
      }
    }
    printf("connection received on %d port\n", ntohs(caddr.sin_port));

    if ((childpid = Fork()) == 0) {
      Close(listenfd);
      str_echo(cfd);
      exit(0);
    }
    Close(cfd);
  }
}