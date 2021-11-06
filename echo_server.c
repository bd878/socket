#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int
Socket(int family, int type, int protocol) {
  int ret;

  if (ret = (socket(family, type, protocol)) < 0) {
    perror("socket");
    exit(1);
  }
  return ret;
}

void
Bind(int fd, const struct sockaddr *addr, size_t len) {
  if (bind(fd, addr, len) < 0) {
    perror("bind");
    exit(1);
  }
}

void
Listen(int fd, int backlog) {
  if (listen(fd, backlog) < 0) {
    perror("listen");
    exit(1);
  }
}

int
Accept(int fd, struct sockaddr *addr, socklen_t *len) {
  int ret;

  if ((ret = accept(fd, addr, len)) < 0) {
    perror("accept");
    exit(1);
  }
  return ret;
}

pid_t
Fork() {
  pid_t p;

  if ((p = fork()) == -1) {
    perror("fork");
    exit(1);
  }
  return p;
}

void
Close(int fd) {
  if (close(fd) == -1) {
    perror("close");
    exit(1);
  }
}

ssize_t
writen(int fd, const void *line, size_t n) {
  size_t nleft;
  ssize_t nwritten;
  const char *vptr;

  vptr = (const char *)line;
  nleft = n;
  while (nleft > 0) {
    if ((nwritten = write(fd, vptr, n)) <= 0) {
      if (nwritten == -1 && errno == EINTR) { /* a signal interrupted write call */
        nwritten = 0;
        continue; /* try again */
      } else {
        return -1;
      }
    }
    nleft -= nwritten;
    vptr += nwritten;
  }
  return n;
}

void
Writen(int fd, const void *line, size_t n) {
  if (writen(fd, line, n) != n) {
    perror("writen");
    exit(1);
  }
}

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