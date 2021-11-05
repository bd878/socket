#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int
main(int argc, char **argv) {
  struct sockaddr_in servaddr, caddr;
  int listenfd, clientfd;
  char cbuff[128];
  socklen_t clen = sizeof(caddr);

  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(1);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(9999);
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  if ((bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) < 0) {
    perror("bind");
    exit(2);
  }

  listen(listenfd, 5);
  printf("Waiting for connections on %d port...\n", ntohs(servaddr.sin_port));

  while (1) {
    clientfd = accept(listenfd, (struct sockaddr *)&caddr, &clen);
    printf("Connection received on %s:%d\n",
      inet_ntop(AF_INET, &caddr.sin_addr, cbuff, sizeof(cbuff)),
      ntohs(caddr.sin_port));

    pid_t pid;
    if ((pid = fork()) == 0) {
      if (close(listenfd) == -1) {
        perror("close");
        exit(4);
      }

      char fdstr[2];
      snprintf(fdstr, sizeof(fdstr), "%d", clientfd);
      printf("forked subprocess with fdstr %s\n", fdstr);
      if (execl("./execed.out", fdstr, (char *)0) == -1) {
        printf("execl error\n");
        perror("execl");
        exit(5);
      }
      printf("returned from forked subprocess\n");
    } else if (pid == -1) {
      perror("fork");
      exit(3);
    }

    printf("forked %d\n", pid);

    close(clientfd);
  }

  return 0;
}