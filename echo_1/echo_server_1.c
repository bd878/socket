#include "../lib.h"

void
str_echo(FILE *fs) {
  static const int MAXLINE = 2;
  char line[MAXLINE];
  ssize_t n;

  while (1) {
    if ((n = fread(line, sizeof(char), MAXLINE, fs)) <= 0) {
      return;
    }

    fwrite(line, sizeof(char), n, fs);
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
      FILE *fs = Fdopen(cfd, "r");
      str_echo(fs);
      exit(0);
    }

    Close(cfd);
  }

  exit(0);
}