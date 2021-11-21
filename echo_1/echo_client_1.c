#include "../lib.h"

void
str_cli(int sockfd) {
  static const int MAXLINE = 1024;
  char line[MAXLINE];
  ssize_t nbackreaden;
  size_t nreaden;
  ssize_t nwritten;
  FILE *sockfile = fdopen(sockfd, "a+");

  errno = 0;
  while ((nreaden = fread(line, 1, MAXLINE, stdin)) > 0) {
    fprintf(stderr, "bytes readen %ld\n", nreaden);
    nwritten = fwrite(line, 1, nreaden, sockfile);
    fprintf(stderr, "bytes written to fd %ld\n", nwritten);

    memset(line, 0, MAXLINE);
    if ((nbackreaden = fread(line, 1, MAXLINE, sockfile)) == 0) {
      printf("EOF reached\n");
      exit(0); /* EOF reached */
    };
    fprintf(stderr, "bytes readen back %ld\n", nbackreaden);

    fwrite(line, 1, nbackreaden, stdout);
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

  str_cli(sockfd);

  exit(0);
};
