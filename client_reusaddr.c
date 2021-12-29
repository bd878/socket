#include "lib.h"

int
main(int argc, char **argv) {
  struct sockaddr_in servaddr, addr;
  int fd, pid;
  int MAXLINE = 1024;
  char line[MAXLINE];

  if (argc != 3) {
    printf("Usage: ./a.out <IPaddress> <IPPort>\n");
    exit(1);
  }

  fd = Socket(AF_INET, SOCK_STREAM, 0);

  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, NULL, 0);

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(1500);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  Bind(fd, (struct sockaddr *)&addr, sizeof(addr));

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(atoi(argv[2]));
  Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

  printf("connecting...\n");

  Connect(fd, (struct sockaddr *)&servaddr, sizeof(servaddr));

  Fgets(line, MAXLINE, stdin);
  Writen(fd, line, strlen(line));

  memset(line, 0, MAXLINE);

  Readline(fd, line, MAXLINE);
  Fputs(line, stdout);

  exit(0);
}
