#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
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
    struct sockaddr_in caddr;
    socklen_t clen = sizeof(caddr);
    char cbuff[128];
    sock = accept(listener, (struct sockaddr *)&caddr, &clen);
    printf("Connection received on %s:%d\n",
      inet_ntop(AF_INET, &caddr.sin_addr, cbuff, sizeof(cbuff)),
      ntohs(caddr.sin_port));

    struct sockaddr_in peercaddr;
    socklen_t peerclen = sizeof(peercaddr);
    char peercbuff[128];
    if (getpeername(sock, (struct sockaddr *)&peercaddr, &peerclen) == -1) {
      perror("getpeername");
      exit(3);
    }
    /* peer connection == accept connection */
    printf("Peer connection is %s:%d\n",
      inet_ntop(AF_INET, &peercaddr.sin_addr, peercbuff, sizeof(peercbuff)),
      ntohs(peercaddr.sin_port));

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