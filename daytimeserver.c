#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int BUF_LEN = 1000;

int main(int argc, char **argv) {
  int listener, sock;
  struct sockaddr_in addr;
  char buff[BUF_LEN];
  time_t ticks;

  listener = socket(AF_INET, SOCK_STREAM, 0);
  if (listener < 0) {
    perror("listen");
    exit(1);
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(9999);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("bind");
    exit(2);
  }

  listen(listener, 1);

  printf("Waiting for connections...\n");
  while (1) {
    sock = accept(listener, NULL, NULL);
    printf("Connection received!\n");

    ticks = time(NULL);
    int written = snprintf(buff, sizeof(buff), "%.24s\er\en", ctime(&ticks));
    for (int i = 0; i < written; ++i) {
      write(sock, &buff[i], sizeof(char));
    }

    close(sock);
    printf("Connection closed\n");
  }

  return 0;
}