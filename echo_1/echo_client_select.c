#include "../lib.h"

void
str_cli(FILE *fp, int fd) {
  static const int MAXLINE = 1024;
  int maxfdp1;
  fd_set rset;
  char sendline[MAXLINE], recvline[MAXLINE];

  int recv_done = 0;
  int send_done = 0;

  FD_ZERO(&rset);
  while (!recv_done || !send_done) {
    FD_SET(fileno(fp), &rset);
    FD_SET(fd, &rset);
    maxfdp1 = max(fileno(fp), fd) + 1;
    Select(maxfdp1, &rset, NULL, NULL, NULL);

    if (FD_ISSET(fd, &rset)) {
      memset(recvline, 0, MAXLINE);
      if (Readline(fd, recvline, MAXLINE) == 0) {
        printf("EOF reached\n");
        recv_done = 1;
      } else {
        Fputs(recvline, stdout);
      }
    }

    if (FD_ISSET(fileno(fp), &rset)) {
      memset(sendline, 0, MAXLINE);
      if (Fgets(sendline, MAXLINE, fp) == NULL) {
        send_done = 1;
      } else {
        Writen(fd, sendline, strlen(sendline));
      }
    }
  }
}

void
str_cli_shutdown(FILE *fp, int fd) {
  static const int MAXLINE = 1024;
  int maxfdp1, fpeof, allsend;
  fd_set rset;
  char buf[MAXLINE];
  int n;

  FD_ZERO(&rset);

  fpeof = 0;
  allsend = 0;
  while (allsend == 0) {
    if (fpeof == 0) {
      FD_SET(fileno(fp), &rset);
    }
    FD_SET(fd, &rset);
    maxfdp1 = max(fileno(fp), fd) + 1;
    Select(maxfdp1, &rset, NULL, NULL, NULL);

    if (FD_ISSET(fd, &rset)) {
      if ((n = Read(fd, buf, MAXLINE)) == 0) {
        if (fpeof == 1) {
          allsend = 1;
        } else {
          printf("std_cli_shutdown: server terminated prematurely\n");
          exit(1);
        }
      } else {
        Writen(fileno(stdout), buf, n);
      }
    }

    if (FD_ISSET(fileno(fp), &rset)) {
      if ((n = Read(fileno(fp), buf, MAXLINE)) == 0) {
        fpeof = 1;
        Shutdown(fd, SHUT_WR);
        FD_CLR(fileno(fp), &rset);
      } else {
        Writen(fd, buf, n);
      }
    }
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

  // str_cli_shutdown(stdin, sockfd);
  str_cli(stdin, sockfd);

  exit(0);
}