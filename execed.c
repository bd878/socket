#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int
main(int argc, char **argv) {
  fprintf(stdout, "entering execed\n");

  if (argc < 1) {
    fprintf(stdout, "Usage: a.out clientfd, received %d argc\n", argc);
    exit(1);
  }

  pid_t clientfd = (pid_t)atoi(argv[0]);
  fprintf(stdout, "received clientfd %d\n", clientfd);

  struct sockaddr_in addr;
  socklen_t len = sizeof(addr);
  if (getpeername(clientfd, (struct sockaddr *)&addr, &len) == -1) {
    perror("getpeername");
    /* close(clientfd); ? */
    exit(2);
  }

  char cbuff[128];
  fprintf(stdout, "Client connection is %s:%d\n",
    inet_ntop(AF_INET, &addr.sin_addr, cbuff, sizeof(cbuff)),
    ntohs(addr.sin_port));

  close(clientfd);
  exit(0);
}