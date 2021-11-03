#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void Listen(int sock, int backlog) {
  char *ptr;

  if ((ptr = getenv("LISTENQ")) != NULL) {
    backlog = atoi(ptr);
  }

  if (listen(sock, backlog) == -1) {
    perror("listen");
    exit(3);
  }
}

void Echo(int fd) {
  char buff[1024];
  int bytes_read;
  while (1) {
    bytes_read = recv(fd, buff, 1024, 0);
    if (bytes_read <= 0) break;
    printf("received %s\n", buff);
    send(fd, buff, bytes_read, 0);
  }
}

int main() {
  int listenfd, clientfd;
  struct sockaddr_in addr, client_addr;
  char buff[128];
  socklen_t len;

  addr.sin_family = AF_INET;
  addr.sin_port = htons(3425);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }

  printf("sock created\n");
  if (bind(listenfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    perror("bind");
    exit(2);
  }

  Listen(listenfd, 10); /* some random backlog value */
  printf("waiting for connections on %s, port %d\n",
    inet_ntop(AF_INET, &addr.sin_addr, buff, sizeof(buff)),
    htons(addr.sin_port)
  );

  while (1) {
    client_addr.sin_family = AF_INET;
    len = sizeof(client_addr);
    if ((clientfd = accept(listenfd, (struct sockaddr *)&client_addr, &len)) == 1) {
      perror("accept");
      exit(4);
    }

    if (getsockname(clientfd, (struct sockaddr *)&client_addr, &len) == -1) {
      perror("getsockname");
      exit(5);
    } else {
      printf("received connection from %s, port %d\n", 
        inet_ntop(AF_INET, &client_addr.sin_addr, buff, sizeof(buff)),
        ntohs(client_addr.sin_port)
      );
    }

    pid_t proc_id;
    if ((proc_id = fork()) == -1) {
      perror("fork");
      exit(6);
    }

    if (proc_id == 0) { /* child proc */
      close(listenfd);
      Echo(clientfd);
      close(clientfd);
      exit(0);
    } else {
      printf("exec client in proc %d\n", proc_id);
    }

    close(clientfd);
  }
}