#include "../lib.h"

void
sig_pipe(int signo) {
  printf("sigpipe raised\n");
  return;
}

void
str_cli(int sockfd) {
  static const int MAXLINE = 1024;
  char line[MAXLINE];
  ssize_t nwritten;

  errno = 0;
  while (Fgets(line, MAXLINE, stdin) != NULL) {
    Writen(sockfd, line, 1);
    sleep(1);
    Writen(sockfd, line + 1, strlen(line) - 1);


    memset(line, 0, MAXLINE);
    if (Readline(sockfd, line, MAXLINE) <= 0) {
      printf("EOF reached\n");
      exit(0); /* EOF reached */
    };

    Fputs(line, stdout);
    memset(line, 0, MAXLINE);
  }

  if (errno != 0) {
    fprintf(stderr, "%s\n", strerror(errno));
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
str_cli_binary(int sockfd) {
  static const int MAXLINE = 1024;
  char line[MAXLINE];
  struct args args;
  struct result result;

  while (Fgets(line, MAXLINE, stdin) != NULL) {
    if (sscanf(line, "%ld%ld", &args.arg1, &args.arg2) != 2) {
      printf("invalid argument %s\n", line);
      continue;
    }

    Writen(sockfd, &args, sizeof(args));
    if (Readn(sockfd, &result, sizeof(result)) == 0) {
      printf("failed to get result\n");
      exit(1);
    }

    printf("%ld\n", result.sum);
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
  Signal(SIGPIPE, sig_pipe);

  str_cli(sockfd);

  exit(0);
};
