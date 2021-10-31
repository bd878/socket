#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAXSOCKADDR 128

int
sockfd_to_family(int sockfd) {
  /* It could be the union { struct sockaddr_in sa; char [MAXSOCKADDR]; } */
  struct sockaddr_storage ss;
  socklen_t len;

  len = MAXSOCKADDR;
  if (getsockname(sockfd, (struct sockaddr *)&ss, &len) < 0) {
    return -1;
  }
  return ss.ss_family;
}

int
main(int argc, char **argv) {
  int sock;
  int family;

  sock = socket(AF_LOCAL, SOCK_DGRAM, 0);
  family = sockfd_to_family(sock);
  printf("%d\n", family);

  close(sock);

  return 0;
}