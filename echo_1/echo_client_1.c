#include "../lib.h"

void
str_cli(FILE *fs, int sockfd) {
  static const int MAXLINE = 2;
  char line[MAXLINE];
  ssize_t n;

  while (fread(line, sizeof(char), MAXLINE, fs) > 0) {
    Writen(sockfd, line, strlen(line));

    memset(line, 0, sizeof(line));
    if ((n = Readline(sockfd, line, MAXLINE)) == 0) {
      exit(0); /* EOF reached */
    };

    Fputs(line, stdout);
  }
}

int main(int argc, char **argv) {
  struct sockaddr_in addr;
  int sockfd;

  if (argc != 2) {
    printf("Usage: ./a.out <IPaddress>\n");
    exit(1);
  }

  sockfd = Socket(AF_INET, SOCK_STREAM, 0);
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(5436);
  Inet_pton(AF_INET, argv[1], &addr.sin_addr);

  Connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
  FILE *fs = Fopen("test.bin", "r");

  str_cli(fs, sockfd);

  exit(0);
};
