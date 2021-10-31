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
  struct sockaddr_in servaddr;
  int sock;
  int family;

  sock = socket(AF_INET, SOCK_STREAM, 0);
  family = sockfd_to_family(sock);
  printf("%d\n", family);

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(13);
  servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

  printf("connecting...\n");
  if (connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) > 0) {
    printf("connection error\n");
    exit(1);
  }
  printf("connected!\n");

  struct sockaddr_in addr;
  socklen_t len = sizeof(addr);
  if (getsockname(sock, (struct sockaddr *)&addr, &len) < 0) { // should set addr.sin_len == sizeof(addr)
    perror("getsockname");
    exit(2);
  }
  char buff[1024];
  printf("connection from %s, port %d\e\n",
    inet_ntop(AF_INET, &addr.sin_addr, buff, sizeof(buff)),
    ntohs(addr.sin_port));

  close(sock);

  return 0;
}