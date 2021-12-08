#include "../lib.h"

int main(int argc, char **argv) {
  struct sockaddr_in addr;
  int sockfd, s;
  struct linger ling;
  ssize_t nwritten;
  char *str = "Hi";

  if (argc == 1) {
    printf("Usage: ./a.out <IPaddress>");
    exit(0);
  }

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  addr.sin_family = AF_INET;
  addr.sin_port = htons(5436);
  if ((s = inet_pton(AF_INET, argv[1], &addr.sin_addr.s_addr)) <= 0) {
    if (s == 0) {
      printf("Wrong ip format %s\n", argv[1]);
      exit(0);
    } else {
      perror("inet_pton");
      exit(1);
    }
  }

  if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    printf("%s\n", strerror(errno));
    exit(1);
  }

  ling.l_onoff = 1;
  ling.l_linger = 0;
  setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));
  Close(sockfd);

  // if ((nwritten = write(sockfd, str, strlen(str))) < 0) {
  //   printf("failed to write %s: %s\n", str, strerror(errno));
  //   exit(1);
  // }

  // printf("%ld bytes written to socked\n", nwritten);
  // close(sockfd);
  exit(0);
}