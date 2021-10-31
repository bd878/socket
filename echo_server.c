#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  int sock, listener;
  struct sockaddr_in addr;
  char buf[1024];
  int bytes_read;

  listener = socket(AF_INET, SOCK_STREAM, 0);
  if (listener < 0) {
    perror("socket");
    exit(1);
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(13);
  addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("bind");
    exit(2);
  }

  listen(listener, 1);
  char buff[128];
  printf("listening on %s, port %d\n",
    inet_ntop(AF_INET, &addr.sin_addr, buff, sizeof(buff)),
    ntohs(addr.sin_port));

  while(1) {
    printf("waiting for connections...\n");
    sock = accept(listener, NULL, NULL);
    printf("received new connection\n");
    if (sock < 0) {
      perror("accept");
      exit(3);
    }

    while (1) {
      bytes_read = recv(sock, buf, 1024, 0);
      if (bytes_read <= 0) break;
      printf("received %s\n", buf);
      send(sock, buf, bytes_read, 0);
    }

    close(sock);
  }

  return 0;
}