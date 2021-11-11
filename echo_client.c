#include "lib.h"

void
str_cli(FILE *fp, int sockfd) {
  static const int MAXLINE = 1024;

  char sendline[MAXLINE], recvline[MAXLINE];

  while (Fgets(sendline, MAXLINE, fp) != NULL) {
    Writen(sockfd, sendline, strlen(sendline));

    if (Readline(sockfd, recvline, MAXLINE) == 0) {
      perror("str_cli");
    }

    Fputs(recvline, stdout);
  }
}

int
main(int argc, char **argv) {
  int sockfd[5];
  struct sockaddr_in servaddr;

  if (argc != 2) {
    perror("usage: a.out <IPaddress>");
    exit(1);
  }

  for (int i = 0; i < 5; ++i) {
    sockfd[i] = Socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9877);
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    Connect(sockfd[i], (struct sockaddr *)&servaddr, sizeof(servaddr));
  }

  str_cli(stdin, sockfd[0]);

  exit(0);
}