#include "../lib.h"

int main() {
  static const int MAXLINE = 1024;
  struct sockaddr_in addr;
  int sockfd, clifd;
  char *str = "Bye!";
  char clistr[MAXLINE];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(5436);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
  listen(sockfd, 1);

  char buf[128];
  inet_ntop(AF_INET, &addr.sin_addr.s_addr, buf, sizeof(buf));
  printf("listening on %s:%d\n", buf, ntohs(addr.sin_port));
  printf("sleeping...\n");
  sleep(5);
  printf("awaken\n");

  while (1) {
    clifd = accept(sockfd, NULL, NULL);

    FILE *cstream = fdopen(clifd, "r+");
    fgets(clistr, MAXLINE, cstream);
    printf("read %ld bytes\n", strlen(clistr));
    printf("%s\n", clistr);
    memset(clistr, 0, sizeof(clistr));

    write(clifd, str, strlen(str));
    close(clifd);
  }

  exit(0);
}