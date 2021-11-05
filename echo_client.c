#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  int sock;
  struct sockaddr_in addr;

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("socket");
    exit(1);
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(9999);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) > 0) {
    perror("connect");
    exit(2);
  }

  char message[] = "Hello there!\n";
  char buff[sizeof(message)];
  send(sock, message, sizeof(message), 0);
  recv(sock, buff, sizeof(message), 0);

  printf("received %s\n", buff);
  close(sock);

  return 0;
}