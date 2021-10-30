#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main() {
  int listenfd, connfd;
  socklen_t len;
  struct sockaddr_in servaddr, cliaddr;
  char buff[1024];
  time_t ticks;

  listenfd = socket(AF_INET, SOCK_STREAM, 0); // TODO: rewrite on Socket with errors handling
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(13); /* daytime server */
  bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
  listen(listenfd, 1);

  char serve_buff[100];
  printf("Listening on ip:port %s:%d\n",
    inet_ntop(AF_INET, &servaddr.sin_addr, serve_buff, sizeof(serve_buff)),
    ntohs(servaddr.sin_port));

  for (;;) {
    len = sizeof(cliaddr);
    connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len);
    printf("connection from %s, port %d\en",
      inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
      ntohs(cliaddr.sin_port));

    ticks = time(NULL);
    snprintf(buff, sizeof(buff), "%24s\er\en", ctime(&ticks));
    write(connfd, buff, strlen(buff));

    close(connfd);
    printf("\nend connection\n");
  }
}