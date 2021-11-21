#include "../lib.h"

void
str_echo(int fd) {
  static const int MAXLINE = 1024;
  char line[MAXLINE];
  ssize_t n;
  FILE *clifile = fdopen(fd, "a+");

  while (1) {
    n = read(fd, line, MAXLINE);
    if (n <= 0) {
      exit(0);
    }
    fprintf(stdout, "chars readen %ld in line %s\n", n, line);

    fwrite(line, 1, n, clifile);
    memset(line, 0, MAXLINE);
  }
}

struct args {
  long arg1;
  long arg2;
};

struct result {
  long sum;
};

void
str_echo_binary(int fd) {
  struct result result;
  struct args args;

  while (1) {
    if (Readn(fd, &args, sizeof(args)) == 0) {
      printf("EOF reached\n");
      exit(0);
    }

    result.sum = args.arg1 + args.arg2;
    Writen(fd, &result, sizeof(result));
  }
}

void
sig_chld(int signo) {
  int status;
  pid_t p;

  while ((p = waitpid(-1, &status, WNOHANG)) != -1) {
    printf("child %d terminated\n", p);
  }
  return;
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
  Signal(SIGCHLD, sig_chld);

  while (1) {
    clen = sizeof(caddr);
    cfd = Accept(sockfd, (struct sockaddr *)&caddr, &clen);

    if ((childpid = Fork()) == 0) {
      Close(sockfd);
      str_echo_binary(cfd);
      exit(0);
    }

    Close(cfd);
  }

  exit(0);
}