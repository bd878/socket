#include "../lib.h"

void
str_cli(int fd, int sockfd) {
  static const int MAXLINE = 1024;
  char line[MAXLINE];
  ssize_t nbackreaden;
  size_t nreaden;
  ssize_t nwritten;

  errno = 0;
  while ((nreaden = read(fd, line, MAXLINE)) > 0) {
    fprintf(stderr, "chars readen %ld, strlen readen line %ld\n", nreaden, strlen(line));
    nwritten = write(sockfd, line, strlen(line));
    fprintf(stderr, "chars written to fd %ld\n", nwritten);

    memset(line, 0, MAXLINE);
    if ((nbackreaden = read(sockfd, line, MAXLINE)) == 0) {
      printf("EOF reached\n");
      exit(0); /* EOF reached */
    };
    fprintf(stderr, "back readen %ld, line size is %ld\n", nbackreaden, strlen(line));

    Fputs(line, stdout);
    memset(line, 0, MAXLINE);
  }

  if (errno != 0) {
    fprintf(stderr, "%s\n", strerror(errno));
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

  str_cli(STDIN_FILENO, sockfd);

  exit(0);
};
